#pragma once
#include <fplus/fplus.hpp>
#include <vector>

namespace SourceParse
{

template<typename T>
struct Tree
{
    T value_;
    std::vector<Tree<T>> children_;
    Tree<T> *parent_ = nullptr;
};


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

} // namespace SourceParse
