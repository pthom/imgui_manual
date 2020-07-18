#include <fplus/fplus.hpp>
#include "source_parse/Sources.h"

using namespace std::literals;

namespace SourceParse
{

LinesWithTags findImGuiDemoCodeLines(const std::string &sourceCode)
{
    static std::string regionToken = "DemoCode";

    auto lines = fplus::split('\n', true, sourceCode);
    auto numberedLines = fplus::enumerate(lines);

    auto demoCodeLines = fplus::keep_if([](const NumberedLine& v) {
        return
                 fplus::is_infix_of("DemoCode"s, v.second)
            && ! fplus::is_prefix_of("#define"s, v.second);
    }, numberedLines);

    LinesWithTags tags = fplus::transform([](const NumberedLine& v) {
        LineWithTag r;
        // codeLine look like "    DemoCode("Menu/Tools");"
        // And for a tag like this, the title should be "Tools", and the level should be 2
        std::string codeLine = v.second;

        r.lineNumber = (int)v.first;
        {
            std::string demoCodeName;
            {
                auto tokens = fplus::split('"', true, codeLine);
                if (tokens.size() >= 3)
                    demoCodeName = tokens[1];
            }
            r.tag = fplus::split('/', false, demoCodeName).back();
            r.level = (int)fplus::count('/', demoCodeName) + 1;
        }
        return r;
    }, demoCodeLines);

    return tags;
}


AnnotatedSource ReadImGuiDemoCode()
{
    std::string sourcePath = "imgui/imgui_demo.cpp";
    AnnotatedSource r;
    r.source = ReadSource(sourcePath);
    r.linesWithTags = findImGuiDemoCodeLines(r.source.sourceCode);
    return r;
}


}