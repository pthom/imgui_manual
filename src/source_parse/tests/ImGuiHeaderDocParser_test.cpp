#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "source_parse/ImGuiHeaderDocParser.h"

TEST_CASE("testing the factorial function")
{
    auto annotatedSource = SourceParse::ReadImGuiHeaderDoc();
    CHECK(annotatedSource.linesWithTags.size() > 10);
}
