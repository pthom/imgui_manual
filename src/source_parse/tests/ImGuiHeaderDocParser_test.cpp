#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "source_parse/ImGuiHeaderDocParser.h"
#include "source_parse/Sources.h"

#include <fplus/fplus.hpp>

TEST_CASE("Test ReadImGuiHeaderDoc")
{
    auto annotatedSource = SourceParse::ReadImGuiHeaderDoc();
    CHECK(annotatedSource.linesWithTags.size() > 10);
}



template<typename T, typename FunctionComparator>
fplus::tree<T> sort_tree(
    FunctionComparator functionComparatorT,
    const fplus::tree<T>& xs)
{
    auto functionComparatorTreeT =
        [&functionComparatorT](const fplus::tree<T> & v1, const fplus::tree<T> & v2)
        -> bool {
        return functionComparatorT(v1.value_, v2.value_);
    };

    fplus::tree<T> r(xs.value_, fplus::sort_by(functionComparatorTreeT, xs.children_));
    return r;
}

template<typename T, typename FunctionComparator>
std::vector<fplus::tree<T>> sort_trees(
    FunctionComparator functionComparatorT,
    const std::vector<fplus::tree<T>>& xs)
{
    auto functionComparatorTreeT =
        [&functionComparatorT](const fplus::tree<T> & v1, const fplus::tree<T> & v2) -> bool
    {
        return functionComparatorT(v1.value_, v2.value_);
    };

    auto treeSortFunction = [&functionComparatorT](const fplus::tree<T>& xxs) {
        return sort_tree(functionComparatorT, xxs);
    };

    auto sorted_children = fplus::transform(
        treeSortFunction,
        xs
    );

    return fplus::sort_by(functionComparatorTreeT, sorted_children);
}


namespace SourceParse
{
    template <typename T>
    struct tree
    {
        T value_;
        std::vector<tree<T>> children_;
        tree<T> *parent_ = nullptr;
    };
    using HeaderTree = tree<LineWithTag>;
    using HeaderTrees = std::vector<HeaderTree>;

    template<typename T>
    std::string show_tree2(const tree<T>& xs)
    {
        std::string siblingSeparator = "\n";
        std::string childStart = "\n<\n";
        std::string childEnd = "\n>";

        auto indentLines = [](const std::string& s, int nbSpaces) -> std::string {
          auto lines = fplus::split_lines(true, s);
          auto indentation = fplus::repeat(nbSpaces, std::string(" "));
          auto indentedLines = fplus::transform([indentation](const auto &ss) { return indentation + ss; }, lines);
          return fplus::join(std::string("\n"), indentedLines);
        };

        std::string r = fplus::show(xs.value_);
        if (!xs.children_.empty())
        {
            auto strings = fplus::transform(show_tree2<T>, xs.children_);
            r = r + childStart +  indentLines(fplus::join(siblingSeparator, strings), 2) + childEnd;
        }
        return r;
    }

    template<typename T>
    std::string show_trees2(const std::vector<tree<T>>& xs)
    {
        auto tree_strings = fplus::transform(show_tree2<T>, xs);
        return fplus::join(std::string("\n"), tree_strings);
    }


    HeaderTree * searchSiblingInParents(HeaderTree* headerTree, int siblingHeaderLevel)
    {
        HeaderTree *possibleSibling = headerTree;
        while (possibleSibling->value_.level > siblingHeaderLevel)
        {
            if (possibleSibling->parent_ != nullptr)
                possibleSibling = possibleSibling->parent_;
            else
                break;
        }
        assert(possibleSibling != nullptr);
        return possibleSibling;
    }

    void makeHeaderTrees_Impl(
        std::deque<LineWithTag>& headerLinesStream,
        HeaderTree& outHeaderTreeParent
        )
    {
        if (headerLinesStream.empty())
            return;

        LineWithTag nextHeader = headerLinesStream.front();
        headerLinesStream.pop_front();

        HeaderTree *parent = nullptr;
        // Search for correct parent
        {
            if (nextHeader.level > outHeaderTreeParent.value_.level)
            {
                // Append child
                parent = &outHeaderTreeParent;
            }
            else if (nextHeader.level == outHeaderTreeParent.value_.level)
            {
                // Create a new sibling tree
                parent = outHeaderTreeParent.parent_;
            }
            else // if (nextHeader.level < outHeaderTreeParent.value_.level)
            {
                // Search for sibling with same level by going up in the parents
                HeaderTree *correctSibling = searchSiblingInParents(&outHeaderTreeParent, nextHeader.level);
                parent = correctSibling->parent_;
            }
        }

        assert(parent != nullptr);
        HeaderTree newTree({nextHeader, {}, parent});
        parent->children_.push_back(newTree);
        HeaderTree& newTreeRef = parent->children_.back();
        makeHeaderTrees_Impl(headerLinesStream, newTreeRef);
    }

    HeaderTrees makeHeaderTrees(const LinesWithTags& linesWithTags)
    {
        std::deque<LineWithTag> headerLinesStream;
        for (const auto& v: linesWithTags)
            headerLinesStream.push_back(v);

        LineWithTag topTag { -1, "", -1};
        HeaderTree  treeTop { topTag, {}, nullptr};
        makeHeaderTrees_Impl(headerLinesStream, treeTop);

        return  treeTop.children_;
    }

} // namespace SourceParse


TEST_CASE("Tree")
{
    SourceParse::LinesWithTags linesWithTags = {
        // { lineNumber, tag, headerLevel }
        {   0, "A", 1 },
        {  10, "A-1", 2 },
        {  20, "A-1-i", 3 },
        {  30, "A-2", 2 },
        {  40, "A-2-i", 3 },
        {  50, "A-2-ii", 3 },
        {  60, "B", 1 },
        {  70, "B-1", 2 },
        {  75, "B-1-i", 3 },
        {  80, "B-2", 2 },
        {  90, "B-3", 2 },
        { 100, "C", 1 },
        { 110, "C-i", 3 },
        { 120, "D", 1 },
    };

    std::string expected =R"(
{line:0, A, level:1}
<
  {line:10, A-1, level:2}
  <
    {line:20, A-1-i, level:3}
  >
  {line:30, A-2, level:2}
  <
    {line:40, A-2-i, level:3}
    {line:50, A-2-ii, level:3}
  >
>
{line:60, B, level:1}
<
  {line:70, B-1, level:2}
  <
    {line:75, B-1-i, level:3}
  >
  {line:80, B-2, level:2}
  {line:90, B-3, level:2}
>
{line:100, C, level:1}
<
  {line:110, C-i, level:3}
>
{line:120, D, level:1}
)";

    {
        auto sourceHeadersTree = SourceParse::makeHeaderTrees(linesWithTags);
        std::cout << SourceParse::show_trees2(sourceHeadersTree) << "\n";
        auto computed = std::string("\n") + show_trees2(sourceHeadersTree) + std::string("\n");
        CHECK(computed == expected);
    }
}

