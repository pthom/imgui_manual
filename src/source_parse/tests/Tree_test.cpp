#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "source_parse/Tree.h"
#include <fplus/fplus.hpp>

using namespace SourceParse;
using namespace std::literals;

Tree<std::string> treeString {
    "Top",
    {
        {"A1"},
        {"A2",
         {
             {"A2i"}
         }
        },
        {"B"},
        {"C"},
    },
};

TEST_CASE("tree_transform_values")
{
    struct StringAndBool
    {
        std::string s;
        bool b;
    };

    auto transform = [](const std::string &s) -> StringAndBool {
        return StringAndBool {s, true};
    };

    auto treeStringAndBool =
        tree_transform_values<StringAndBool>(transform, treeString);
}

TEST_CASE("treeKeepIfAnyAncesterMatches")
{
    auto matchA = [](const std::string &s) -> bool {
      return fplus::is_prefix_of("A"s, s);
    };

    Tree<std::string> treeFiltered = treeKeepIfAnyAncesterMatches(matchA, treeString);
    std::cout << showTree(treeFiltered) << "\n";
}