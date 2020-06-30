#include "hello_imgui/hello_imgui_assets.h"
#include <fplus/fplus.hpp>
#include "Sources.h"

using namespace std::literals;

namespace {
    auto make_string_vec = [](const std::string &s) -> std::vector<std::string> {
        std::vector<std::string> r = fplus::split('\n', false, s);
        r = fplus::transform(fplus::trim_whitespace<std::string>, r);
        r = fplus::keep_if([](auto s) { return !s.empty();}, r);
        return r;
    };
}

namespace Sources
{


std::vector<Library> imguiLibrary()
{
    return
    {
        {
            "imgui", "Dear ImGui", "https://github.com/ocornut/imgui",
            make_string_vec(R"(
                README.md
                FAQ.md
                LICENSE.txt
                imgui_demo.cpp
                imgui.h
                imgui.cpp
                imconfig.h
                imgui_draw.cpp
                imgui_internal.h
                imgui_widgets.cpp
                imstb_rectpack.h
                imstb_textedit.h
                imstb_truetype.h
            )")
        }
    };
}

std::vector<Library> thisDemoLibraries()
{
    return
    {
        {
            "imgui_hellodemo", "This Demo", "https://github.com/pthom/implot_demo",
            make_string_vec(R"(
                Readme.md
                LICENSE
                ImGuiHelloDemo.cpp
                Sources.h
                Sources.cpp
        )")
        },
    };
}

std::vector<Library> otherLibraries()
{
    return
    {
        {
            "hello_imgui", "Hello ImGui", "https://github.com/pthom/hello_imgui",
            make_string_vec(R"(
                README.md
                LICENSE
                hello_imgui.h
                hello_imgui_api.md
            )")
        },
        {
            "ImGuiColorTextEdit", "ImGuiColorTextEdit", "https://github.com/BalazsJako/ImGuiColorTextEdit",
            make_string_vec(R"(
                README.md
                LICENSE
                CONTRIBUTING
                TextEditor.h
                TextEditor.cpp
            )")
        },
        {
            "imgui_markdown", "imgui_markdown", "https://github.com/juliettef/imgui_markdown",
            make_string_vec(R"(
                README.md
                License.txt
                imgui_markdown.h
            )")
        },
        {
                "fplus", "FunctionalPlus", "https://github.com/Dobiasd/FunctionalPlus",
                make_string_vec(R"(
                    README.md
                    LICENSE
                    CONTRIBUTING.md
                    INSTALL.md
                    fplus.hpp
            )")
        },
    };
}


std::string lowerCaseExceptFirstLetter(const std::string &s)
{
    auto r = fplus::to_upper_case(fplus::take(1, s)) +  fplus::to_lower_case(fplus::drop(1, s));
    return r;
}

std::string upperCaseQAndA(const std::string &s)
{
    if (fplus::take(3, s) == "Q&a")
        return "Q&A"s + fplus::drop(3, s);
    else
        return s;
}

std::string lowerCaseTitle(const std::string& s)
{
    auto words = fplus::split(' ', true, s);
    words = fplus::transform(lowerCaseExceptFirstLetter, words);
    auto title = fplus::join(" "s, words);
    title = upperCaseQAndA(title);
    return title;
}

LinesWithTags findImGuiCppDoc(const std::string &sourceCode)
{
    LinesWithTags r;
    /*
      H1 titles look like this
      MISSION STATEMENT
      =================
      H2 titles look like this
      READ FIRST
     ----------
   */
    auto lines = fplus::split('\n', true, sourceCode);

    // Given two lines, we can check whether they are a header
    // and return 0 (not header) , 1 ("H1") or 2 ("H2")
    auto isHeaderLine = [](const std::pair<std::string, std::string> &linePair) {
        int r = 0;
        if (fplus::is_prefix_of("===="s, fplus::trim_whitespace(linePair.second)))
            r = 1;
        if (fplus::is_prefix_of("----"s, fplus::trim_whitespace(linePair.second)))
            r = 2;
        return r;
    };

    for (auto idx_lines : fplus::enumerate(fplus::overlapping_pairs(lines)))
    {
        int lineNumber = (int)idx_lines.first + 1;
        auto line_pair = idx_lines.second;
        int headerWeight = isHeaderLine(line_pair);
        std::string tag = fplus::trim_whitespace(line_pair.first);
        tag = lowerCaseTitle(tag);
        if (headerWeight == 1)
            r.push_back({lineNumber, "H1 "s + tag});
        if (headerWeight == 2)
            r.push_back({lineNumber, "H2 "s + tag});
    }
    return r;
}


LinesWithTags findImGuiDemoCodeLines(const std::string &sourceCode)
{
    LinesWithTags r;

    static std::string regionToken = "DemoCode(";

    auto extractDemoCodeName = [](const std::string &codeLine) {
        // if codeLine == "DemoCode("Line Plots")) {"
        // then return
        // "Line Plots"
        auto tokens = fplus::split('"', true, codeLine);
        if (tokens.size() >= 3)
            return tokens[1];
        else
            return std::string();
    };

    auto lines = fplus::split('\n', true, sourceCode);
    for (size_t line_number = 0; line_number < lines.size(); line_number++)
    {
        const std::string& line = lines[line_number];
        if (line.find(regionToken) != std::string::npos)
            r.push_back({(int)line_number + 1, extractDemoCodeName(line)});
    }
    return r;
}

Source ReadSource(const std::string sourcePath)
{
    std::string assetPath = std::string("code/") + sourcePath;
    auto assetData = HelloImGui::LoadAssetFileData(assetPath.c_str());
    assert(assetData.data != nullptr);

    Source r;
    r.sourcePath = sourcePath;
    r.sourceCode = std::string((const char *) assetData.data);
    HelloImGui::FreeAssetFileData(&assetData);
    return r;
}


AnnotatedSource ReadImGuiDemoCode(const std::string& sourcePath)
{
    AnnotatedSource r;
    r.source = ReadSource(sourcePath);
    r.linesWithTags = findImGuiDemoCodeLines(r.source.sourceCode);
    return r;
}

AnnotatedSource ReadImGuiCppDoc(const std::string& sourcePath) {
    AnnotatedSource r;
    r.source = ReadSource(sourcePath);
    r.linesWithTags = findImGuiCppDoc(r.source.sourceCode);
    return r;
}
} // namespace Sources
