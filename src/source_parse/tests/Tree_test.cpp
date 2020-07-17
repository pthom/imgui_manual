#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "source_parse/Tree.h"
#include <fplus/fplus.hpp>
#include <ostream>

using namespace SourceParse;
using namespace std::literals;

struct StringAndBool
{
    std::string s;
    bool b;
};

std::ostream& operator<<(std::ostream &os, const StringAndBool &v)
{
    os << "{" << v.s << " : " << v.b << "}";
    return os;
}

TEST_CASE("tree_transform_values")
{
    Tree<std::string> treeString = tree_from_string<std::string>(
        fplus::trim_whitespace(R"(
Top
  A1
  A2
    A2i
    A3i
  B
  C
    )"s));
    // std::cout << tree_show(treeString);
    auto toStringAndBool = [](const std::string &s) -> StringAndBool {
        return StringAndBool {s, true};
    };
    auto treeStringAndBool = tree_transform_values<StringAndBool>(toStringAndBool, treeString);
    treeStringAndBool.value_.b = false;
    std::string expected = R"(
{Top : 0}
  {A1 : 1}
  {A2 : 1}
    {A2i : 1}
    {A3i : 1}
  {B : 1}
  {C : 1}
)";
    // std::cout << tree_show(treeStringAndBool) << "\n";
    CHECK("\n"s + tree_show(treeStringAndBool) + "\n"s == expected );
}

TEST_CASE("tree_keepIfAnyAncesterMatches")
{
    Tree<std::string> treeString = tree_from_string<std::string>(
        fplus::trim_whitespace(R"(
Top
  A1
  A2
    A2i
    A3i
  B
  C
    )"s));

    auto containsA2 = [](const std::string &s) -> bool {
      return fplus::is_infix_of("A2"s, s);
    };

    Tree<std::string> treeFiltered = tree_keep_wholebranch_if(containsA2, treeString);
    // std::cout << tree_show(treeFiltered) << "\n";
    std::string expected = R"(
Top
  A2
    A2i
    A3i
)";
    CHECK("\n"s + tree_show(treeFiltered) + "\n"s == expected );
}


TEST_CASE("tree_from_string")
{
    std::string treeAsString = R"(
Top
  A
    A1
      A1i
      A1ii
  B
    B1
    B2
    B3
  C
  D
)";
    auto tree = tree_from_string<std::string>(treeAsString);
    // std::cout << tree_show(tree) << "\n";
    std::string treeAsString2 = "\n"s + tree_show(tree) + "\n";
    CHECK(treeAsString == treeAsString2);
}