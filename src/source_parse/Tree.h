#pragma once
#include <fplus/fplus.hpp>
#include <vector>
#include <ostream>

namespace SourceParse
{

template<typename T>
struct Tree
{
    T value_;
    std::vector<Tree<T>> children_ = {};
};

template <typename T, typename Visitor_T>
inline void tree_visit_breadth_first(Visitor_T visitor, Tree<T> & xs_io)
{
    visitor(xs_io);
    for (auto & child : xs_io.children_)
        tree_visit_breadth_first(visitor, child);
}

template <typename T, typename Visitor_T>
inline void tree_visit_depth_first(Visitor_T visitor, Tree<T> & xs_io)
{
    for (auto & child : xs_io.children_)
        tree_visit_depth_first(visitor, child);
    visitor(xs_io);
}

template <typename T>
Tree<T>* tree_find_parent(Tree<T>& treeRoot, Tree<T>& treeNode)
{
    Tree<T> * parent = nullptr;
    auto visitor = [&parent, &treeNode](Tree<T>& xs) {
        for (auto& child : xs.children_)
            if (&child == &treeNode)
                parent = &xs;
    };
    tree_visit_breadth_first(visitor, treeRoot);
    return parent;
}

// Apply a visitor to node ancestors (but *not* to the node itself)
template <typename T, typename Visitor_T>
inline void tree_visit_parents(
    Visitor_T visitor,
    Tree<T> & tree_root,
    Tree<T> & node)
{
    Tree<T>* parent = &node;
    do
    {
        parent = tree_find_parent<T>(tree_root, *parent);
        if (parent)
            visitor(*parent);
    } while(parent != nullptr);
}


template<typename T, typename FunctionComparator>
inline Tree<T> tree_sort(
    FunctionComparator functionComparatorT,
    const Tree<T>& xs)
{
    auto functionComparatorTreeT =
        [&functionComparatorT](const Tree<T> & v1, const Tree<T> & v2)
            -> bool {
          return functionComparatorT(v1.value_, v2.value_);
        };

    auto treeSortFunction = [&functionComparatorT](const Tree<T>& xxs) {
      return tree_sort(functionComparatorT, xxs);
    };

    auto sorted_children = fplus::transform(
        treeSortFunction,
        xs.children_
    );

    Tree<T> r{xs.value_, fplus::sort_by(functionComparatorTreeT, sorted_children)};
    return r;
}


template<typename T>
inline std::string tree_show(const Tree<T>& xs)
{
    std::string siblingSeparator = "\n";
    std::string childStart = "\n";
    std::string childEnd = "";

    auto indentLines = [](const std::string& s, int nbSpaces) -> std::string {
      auto lines = fplus::split_lines(true, s);
      auto indentation = fplus::repeat(nbSpaces, std::string(" "));
      auto indentedLines = fplus::transform([indentation](const auto &ss) { return indentation + ss; }, lines);
      return fplus::join(std::string("\n"), indentedLines);
    };

    std::string r = fplus::show(xs.value_);
    if (!xs.children_.empty())
    {
        auto strings = fplus::transform(tree_show<T>, xs.children_);
        r = r + childStart +  indentLines(fplus::join(siblingSeparator, strings), 2) + childEnd;
    }
    return r;
}


template <typename T, typename UnaryPredicate>
inline Tree<T> tree_keep_if(UnaryPredicate f, const Tree<T> & xs)
{
    Tree<T> result = xs;

    result.children_ = fplus::keep_if(
        [f](const Tree<T> & xxs) { return f(xxs.value_); },
        result.children_);

    std::vector<Tree<T>> children_filtered_grandchhildren;
    for (auto & child : result.children_)
    {
        auto child_filtered = tree_keep_if(f, child);
        children_filtered_grandchhildren.push_back(child_filtered);
    }
    result.children_ = children_filtered_grandchhildren;
    return result;
}


template <typename OutType, typename T, typename Transformer_T>
inline Tree<OutType> tree_transform_values(
    Transformer_T transformer, const Tree<T> & xs
    )
{
    Tree<OutType> r;
    r.value_ = transformer(xs.value_);
    for (auto & child : xs.children_)
        r.children_.push_back(tree_transform_values<OutType>(transformer, child));
    return r;
}

template<typename T>
struct FlaggedValue
{
    T value;
    bool flag;
};

template<typename T>
std::ostream & operator<<(std::ostream& os, const FlaggedValue<T> &v)
{
    os << "{" << v.value << " : " << v.flag << "}";
    return os;
}


// Filters a tree by keeping whole branches (from the root to the leafs)
// where any node matches the predicate
template <typename T, typename UnaryPredicate>
inline Tree<T> tree_keep_wholebranch_if(UnaryPredicate f, const Tree<T> & xs)
{
    // Step 1: transform the tree into the same tree, but with flags
    auto toFlaggedValue = [](const T& v) {
        return FlaggedValue<T>{v, false};
    };
    Tree<FlaggedValue<T>> flaggedTree = tree_transform_values<FlaggedValue<T>>(toFlaggedValue, xs);

    // This visitor will apply the flag on a given node + all its parent and descendants
    // if the predicate returns true
    auto visitorFlagBranch =
        [&f, &flaggedTree](Tree<FlaggedValue<T>>& v)
    {
        auto visitorSetFlagOn = [&f](Tree<FlaggedValue<T>>& v)
        {
            v.value_.flag = true;
        };

        bool flag = f(v.value_.value);
        if (flag)
        {
            tree_visit_parents(visitorSetFlagOn, flaggedTree, v);
            tree_visit_breadth_first(visitorSetFlagOn, v);
        }
    };

    // We flag the whole tree by branch
    tree_visit_breadth_first(visitorFlagBranch, flaggedTree);
    //std::cout << tree_show(flaggedTree) << "\n";

    // keep only the flagged node
    auto filteredFlaggedTree = tree_keep_if([](auto v) { return v.flag; }, flaggedTree);

    // And then we retransform the flagged tree into a standard tree,
    auto toT = [](const FlaggedValue<T>& v) {
      return v.value;
    };
    auto filteredTree = tree_transform_values<T>(toT, filteredFlaggedTree);

    return filteredTree;
}


template<typename T>
T value_from_string(const std::string & s)
{
    T value;
    std::istringstream ss(s);
    ss >> value;
    return value;
}


template<typename T>
Tree<T> tree_from_string(const std::string &s, int indentation = 2)
{
    Tree<T> root;
    Tree<T> *current_node = &root;
    auto lines = fplus::split_lines(false, s);
    int current_depth = 0;
    bool was_root_already_added = false;
    for (const auto& line: lines)
    {
        int line_depth = fplus::take_while([](auto c){return c == ' ';}, line).size() / indentation;
        std::string trimmed_line = fplus::trim_whitespace_left(line);
        T value = value_from_string<T>(trimmed_line);
        Tree<T> value_node{value};
        if (line_depth == 0)
        {
            assert(!was_root_already_added);
            was_root_already_added = true;
            root.value_ = value;
        }
        else if (line_depth == current_depth)
        {
            current_node->children_.push_back(value_node);
        }
        else if (line_depth > current_depth)
        {
            assert((line_depth - current_depth) == 1);
            current_node = &(current_node->children_.back());
            current_node->children_.push_back(value_node);
        }
        else if (line_depth < current_depth)
        {
            int nbUps = current_depth - line_depth;
            for (int i = 0; i < nbUps; ++i)
            {
                Tree<T>* parent = tree_find_parent(root, *current_node);
                assert(parent != nullptr);
                current_node = parent;
            }
            current_node->children_.push_back(value_node);
        }
        current_depth = (line_depth == 0) ? 1 : line_depth;
    }
    return root;
}


} // namespace SourceParse
