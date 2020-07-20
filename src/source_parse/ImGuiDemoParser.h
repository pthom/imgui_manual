#pragma once
#include <unordered_map>
#include "source_parse/Sources.h"

namespace SourceParse
{
    AnnotatedSource ReadImGuiDemoCode();
    std::unordered_map<std::string, SourceCode> FindExampleAppsCode();
} // namespace SourceParse
