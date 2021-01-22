#include <fplus/fplus.hpp>
#include "source_parse/Sources.h"

using namespace std::literals;

namespace SourceParse
{

LinesWithTags findImGuiDemoCodeLines(const std::string &sourceCode)
{
    static std::string regionToken = "DEMO_MARKER";

    auto lines = fplus::split('\n', true, sourceCode);
    auto numberedLines = fplus::enumerate(lines);

    auto demoCodeLines = fplus::keep_if([](const NumberedLine& v) {
        return
                 fplus::is_infix_of("DEMO_MARKER"s, v.second)
            && ! fplus::is_prefix_of("#define"s, v.second)
            && ! fplus::is_infix_of("ImGui::SetTooltip"s, v.second);
    }, numberedLines);

    LinesWithTags tags = fplus::transform([](const NumberedLine& v) {
        LineWithTag r;
        // codeLine look like "    DEMO_MARKER("Menu/Tools");"
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

std::unordered_map<std::string, SourceCode> FindExampleAppsCode()
{
    std::string sourcePath = "imgui/imgui_demo.cpp";
    auto sourceFile = ReadSource(sourcePath);

    // Example apps sections look like this
    /*
//-----------------------------------------------------------------------------
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------
// - ShowExampleAppMainMenuBar()
// - ShowExampleMenuFile()
//-----------------------------------------------------------------------------
     */

    // Step 1 : find all section starts
    auto numberedLines = fplus::enumerate(fplus::split_lines(true, sourceFile.sourceCode));
    auto isLineExampleAppSectionStarts = [&numberedLines](const NumberedLine& numberedLine) {
        size_t lineNumber = numberedLine.first;
        if (lineNumber >= numberedLines.size() - 1)
            return false;
        std::string codeLine = numberedLines[lineNumber].second;
        std::string nextCodeLine = numberedLines[lineNumber +  1].second;
        if (! fplus::is_prefix_of("//-----------------"s, codeLine))
            return false;
        if (! fplus::is_prefix_of("// [SECTION] Example App:"s, nextCodeLine))
            return false;
        return true;
    };
    auto idxLineStartExampleSections = fplus::find_all_idxs_by(isLineExampleAppSectionStarts, numberedLines);

    // Step 2: add an additional index which is the end of the demo code in imgui_demo.cpp
    // it is denoted by the following line:
    /*
// End of Demo code
    */
    {
        auto endLineNumber = fplus::find_first_idx_by([](const NumberedLine& line){
            return line.second == "// End of Demo code";
        }, numberedLines);
        assert(endLineNumber.is_just());
        idxLineStartExampleSections.push_back(endLineNumber.unsafe_get_just());
    }

    // Step 3: find the sections scopes (i.e start/end lines)
    std::vector<std::pair<size_t, size_t>> sectionScopes = fplus::overlapping_pairs(idxLineStartExampleSections);

    // Step 4: Gather the sections into a map
    std::unordered_map<std::string, SourceCode> exampleAppsCodes;
    auto extractSectionName = [&numberedLines](const std::pair<size_t, size_t>& sectionScope)
    -> std::string {
        auto titleLine = numberedLines[sectionScope.first + 1].second;
        // titleLine looks like this:
        // [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
        auto items = fplus::split('/', true, titleLine);
        assert(items.size() > 1);

        std::string functionName = fplus::trim_whitespace(items.back());
        // functionName should look like ShowExampleAppMainMenuBar()
        std::string appName = fplus::replace_tokens("Show"s, ""s, functionName);
        appName = fplus::replace_tokens("()"s, ""s, appName);
        return appName;
    };

    for (const auto& sectionScope: sectionScopes)
    {
        std::string sectionName = extractSectionName(sectionScope);
        SourceCode sectionCode;
        {
            auto sectionNumberedLines =
                fplus::get_segment(sectionScope.first, sectionScope.second, numberedLines);
            std::vector<std::string> sectionLines =
                fplus::transform([](const auto&v) { return v.second; }, sectionNumberedLines);
            sectionCode = fplus::join("\n"s, sectionLines);
        }
        exampleAppsCodes[sectionName] = sectionCode;
    }
    return exampleAppsCodes;
}

}