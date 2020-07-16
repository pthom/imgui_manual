#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "source_parse/ImGuiHeaderDocParser.h"
#include "source_parse/Sources.h"
#include "source_parse/HeaderTree.h"
#include <fplus/fplus.hpp>

TEST_CASE("Test ReadImGuiHeaderDoc")
{
    auto annotatedSource = SourceParse::ReadImGuiHeaderDoc();
    CHECK(annotatedSource.linesWithTags.size() > 10);
}
