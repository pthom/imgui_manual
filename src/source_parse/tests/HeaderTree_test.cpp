#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "source_parse/HeaderTree.h"


TEST_CASE("makeHeaderTrees")
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
{line:-1, TOC, level:-1}
  {line:0, A, level:1}
    {line:10, A-1, level:2}
      {line:20, A-1-i, level:3}
    {line:30, A-2, level:2}
      {line:40, A-2-i, level:3}
      {line:50, A-2-ii, level:3}
  {line:60, B, level:1}
    {line:70, B-1, level:2}
      {line:75, B-1-i, level:3}
    {line:80, B-2, level:2}
    {line:90, B-3, level:2}
  {line:100, C, level:1}
    {line:110, C-i, level:3}
  {line:120, D, level:1}
)";

    auto sourceHeadersTree = SourceParse::makeHeaderTree(linesWithTags);
    auto computed = std::string("\n") + showTree(sourceHeadersTree) + std::string("\n");
    //std::cout << computed;
    CHECK(computed == expected);
}

TEST_CASE("sortHeaderTrees")
{
    SourceParse::LinesWithTags linesWithTags = {
        // { lineNumber, tag, headerLevel }
        {   0, "A", 1 },
        {  10, "A-1", 2 },
        {  20, "A-2", 2 },
        {  21, "A-2-i", 3 },
        {  22, "A-2-ii", 3 },
        {  23, "A-2-iii", 3 },
        {  30, "B", 1 },
        {  40, "B-1", 2 },
        {  50, "B-2", 2 },
    };

    std::string expected = R"(
{line:-1, TOC, level:-1}
  {line:30, B, level:1}
    {line:50, B-2, level:2}
    {line:40, B-1, level:2}
  {line:0, A, level:1}
    {line:20, A-2, level:2}
      {line:23, A-2-iii, level:3}
      {line:22, A-2-ii, level:3}
      {line:21, A-2-i, level:3}
    {line:10, A-1, level:2}
)";

    auto sourceHeadersTrees = SourceParse::makeHeaderTree(linesWithTags);
    auto fnComparator = [](const SourceParse::LineWithTag& t1, const SourceParse::LineWithTag& t2)
    {
      return t2.lineNumber < t1.lineNumber;
    };
    auto sourceHeadersTreesReversed = SourceParse::sortTree(fnComparator, sourceHeadersTrees);
    auto computed = std::string("\n") + SourceParse::showTree(sourceHeadersTreesReversed) + std::string("\n");
    //std::cout << computed;
    CHECK(computed == expected);
}


TEST_CASE("badHierarchyTree")
{
    SourceParse::LinesWithTags linesWithTags = {
        // { lineNumber, tag, headerLevel }
        {   0, "A", 1 },
        {  20, "A-1-i", 3 }, // bad hierarchy, because level 3 is reached without level 2
        {  30, "A-2", 2 },   // and this level 2 now should becomes a sibling of previous level 3
        {  40, "A-2-i", 3 },
        {  60, "B", 1 },
    };
    std::string expected = R"(
{line:-1, TOC, level:-1}
  {line:0, A, level:1}
    {line:20, A-1-i, level:3}
    {line:30, A-2, level:2}
      {line:40, A-2-i, level:3}
  {line:60, B, level:1}
)";

    auto sourceHeadersTree = SourceParse::makeHeaderTree(linesWithTags);
    auto computed = std::string("\n") + showTree(sourceHeadersTree) + std::string("\n");
    //std::cout << computed;
    CHECK(computed == expected);
}
