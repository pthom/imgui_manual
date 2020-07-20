#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "source_parse/ImGuiDemoParser.h"
#include <fplus/fplus.hpp>

using namespace SourceParse;

TEST_CASE("Test FindExampleAppsCode")
{
    std::unordered_map<std::string, SourceCode> apps = FindExampleAppsCode();
    CHECK_GE(apps.size(), 13);
}
