#pragma once
#include <fplus/fplus.hpp>
#include <vector>
//#include <list>

namespace SourceParse
{

template<typename T>
struct Tree
{
    T value_;
    std::vector<Tree<T>> children_ = {};
    Tree<T> *parent_ = nullptr;
};

/*
template<typename T>
struct Tree
{
    std::vector<T> nodes_;
    TreeNode<T> root_;
};

template<typename T>
class TreeNode
{
public:
    TreeNode(const T& value) : value_(value){ }
    T& value() { return value_; }
    const T& const value() { return value_; }
    const std::vector<T&> children() { return children;};
    void addChild(const T& child) { children_push_back(child); }
private:
    T value_;
    std::vector<TreeNode<T>> children_ = {};
    TreeNode<T> *parent_ = nullptr;
};

*/



template<typename T, typename FunctionComparator>
inline Tree<T> sortTree(
    FunctionComparator functionComparatorT,
    const Tree<T>& xs)
{
    auto functionComparatorTreeT =
        [&functionComparatorT](const Tree<T> & v1, const Tree<T> & v2)
            -> bool {
          return functionComparatorT(v1.value_, v2.value_);
        };

    auto treeSortFunction = [&functionComparatorT](const Tree<T>& xxs) {
      return sortTree(functionComparatorT, xxs);
    };

    auto sorted_children = fplus::transform(
        treeSortFunction,
        xs.children_
    );

    Tree<T> r{xs.value_, fplus::sort_by(functionComparatorTreeT, sorted_children), xs.parent_};
    return r;
}


template<typename T>
inline std::string showTree(const Tree<T>& xs)
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
        auto strings = fplus::transform(showTree<T>, xs.children_);
        r = r + childStart +  indentLines(fplus::join(siblingSeparator, strings), 2) + childEnd;
    }
    return r;
}

template <typename T, typename Transformer_T>
inline void tree_transform_leafs_depth_first_inplace(Transformer_T transformer, Tree<T> & xs_io)
{
    for (auto & child : xs_io.children_)
        tree_transform_leafs_depth_first_inplace(transformer, child);
    transformer(xs_io);
}

template <typename T, typename Transformer_T>
inline void tree_transform_leafs_breadth_first_inplace(Transformer_T transformer, Tree<T> & xs_io)
{
    transformer(xs_io);
    for (auto & child : xs_io.children_)
        tree_transform_leafs_breadth_first_inplace(transformer, child);
}


template <typename T, typename UnaryPredicate>
inline Tree<T> treeKeepIf(UnaryPredicate f, const Tree<T> & xs)
{
    Tree<T> result = xs;

    result.children_ = fplus::keep_if(
        [f](const Tree<T> & xxs) { return f(xxs.value_); },
        result.children_);

    std::vector<Tree<T>> children_filtered_grandchhildren;
    for (auto & child : result.children_)
    {
        auto child_filtered = treeKeepIf(f, child);
        children_filtered_grandchhildren.push_back(child_filtered);
    }
    result.children_ = children_filtered_grandchhildren;
    return result;
}


template <typename OutType, typename T, typename Transformer_T>
inline Tree<OutType> tree_transform_values(
    Transformer_T transformer, const Tree<T> & xs,
    Tree<OutType> *parent = nullptr
    )
{
    Tree<OutType> r;
    r.value_ = transformer(xs.value_);
    r.parent_ = parent;
    for (auto & child : xs.children_)
        r.children_.push_back(tree_transform_values<OutType>(transformer, child, &r));
    return r;
}

template <typename T, typename UnaryPredicate>
inline Tree<T> treeKeepIfAnyAncesterMatches(UnaryPredicate f, const Tree<T> & xs)
{
    // Remove all children for which none of the parent, grand parent, etc matches
    struct FlaggedT
    {
        T value;
        bool flag;
    };
    auto toFlaggedT = [](const T& v) {
        return FlaggedT {v, false};
    };

    auto flaggedTree = tree_transform_values<FlaggedT>(toFlaggedT, xs);

    auto flagIfNodeOrAnyAncesterMatchPredicate = [f](Tree<FlaggedT> &tree)
    {
        if (f(tree.value_.value))
        {
            tree.value_.flag = true;
            return;
        }
        Tree<FlaggedT> *parent = tree.parent_;
        while(parent != nullptr)
        {
            if (parent->value_.flag)
            {
                tree.value_.flag = true;
                return;
            }
            parent = parent->parent_;
        }
    };

    tree_transform_leafs_breadth_first_inplace(
        flagIfNodeOrAnyAncesterMatchPredicate, flaggedTree);

    auto filteredFlaggedTree = treeKeepIf([](auto v) { return v.flag; }, flaggedTree);

    auto toT = [](const FlaggedT& v) {
      return v.value;
    };

    auto filteredTree = tree_transform_values<T>(toT, filteredFlaggedTree);

    return filteredTree;
}


} // namespace SourceParse
