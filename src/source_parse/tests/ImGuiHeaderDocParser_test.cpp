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


template<typename T>
std::string show_tree(const fplus::tree<T>& xs)
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
        auto strings = fplus::transform(show_tree<T>, xs.children_);
        r = r + childStart +  indentLines(fplus::join(siblingSeparator, strings), 2) + childEnd;
    }
    return r;n r2;
    //return r;
}

template<typename T>
std::string show_trees(const std::vector<fplus::tree<T>>& xs)
{
    auto tree_strings = fplus::transform(show_tree<T>, xs);
    return fplus::join(std::string("\n"), tree_strings);
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


std::vector<fplus::tree<SourceParse::LineWithTag>> SourceHeadersTree(
    const SourceParse::LinesWithTags linesWithTags
    )
{
    auto isTagChildOf =
        [&linesWithTags](const SourceParse::LineWithTag& testedChild,
                         const SourceParse::LineWithTag& testedParent)
        {
          if (testedChild.level <= testedParent.level)
              return false;
          // The possible parent should appear before this tag
          // and have a higher level
          std::vector<SourceParse::LineWithTag> allPossibleParents =
              fplus::keep_if(
                  [&testedChild](const SourceParse::LineWithTag & possibleParent ) {
                    return (
                        possibleParent.level < testedChild.level
                        && possibleParent.lineNumber < testedChild.lineNumber);
                  },
                  linesWithTags);
          if (allPossibleParents.empty())
              return false;

          // The best parent is the parent with the highest line number
          // amongst the parents
          SourceParse::LineWithTag bestParent = fplus::partial_sort_by(
              [](const auto& t1, const auto& t2) {
                return t1.lineNumber > t2.lineNumber;
              },
              1,
              allPossibleParents
          ).front();

          return testedParent.lineNumber == bestParent.lineNumber;
        };

    std::vector<fplus::tree<SourceParse::LineWithTag>> trees =
                                                           fplus::trees_from_sequence(isTagChildOf, linesWithTags);

    auto cmpTags = [](const SourceParse::LineWithTag& t1, const SourceParse::LineWithTag& t2) {
      return t1.lineNumber < t2.lineNumber;
    };
    auto trees_sorted = sort_trees(cmpTags, trees);

    return trees_sorted;
}

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


    auto sourceHeadersTree = SourceHeadersTree(linesWithTags);
    std::cout << show_trees(sourceHeadersTree) << "\n";
    auto computed = std::string("\n") + show_trees(sourceHeadersTree) + std::string("\n");
    CHECK(computed == expected);
}

