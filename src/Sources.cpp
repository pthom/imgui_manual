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
            "Dear ImGui: Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies",
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

std::vector<Library> otherLibraries()
{
    return
    {
        {
            "ImGuiColorTextEdit", "ImGuiColorTextEdit", "https://github.com/BalazsJako/ImGuiColorTextEdit",
            "Syntax highlighting text editor for ImGui. Demo project: [https://github.com/BalazsJako/ColorTextEditorDemo](https://github.com/BalazsJako/ColorTextEditorDemo)",
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
            "Markdown for Dear ImGui. [Become a Patron](https://www.patreon.com/enkisoftware)",
            make_string_vec(R"(
                README.md
                License.txt
                imgui_markdown.h
            )")
        },
        {
            "fplus", "FunctionalPlus", "https://github.com/Dobiasd/FunctionalPlus",
            "Functional Programming Library for C++. Write concise and readable C++ code. [API Browser](http://www.editgym.com/fplus-api-search/) - [Udemy course](https://www.udemy.com/course/functional-programming-using-cpp/) - [Browse the code](https://sourcegraph.com/github.com/Dobiasd/FunctionalPlus/-/tree/include/fplus). ",
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


std::vector<Library> helloImGuiLibrary()
{
    return
    {
        {
            "hello_imgui", "Hello ImGui", "https://github.com/pthom/hello_imgui",
            "Hello, Dear ImGui: cross-platform Gui apps for Windows / Mac / Linux / iOS / Android / Emscripten with the simplicity of a \"Hello World\" app",
            make_string_vec(R"(
                README.md
                LICENSE
                hello_imgui.h
                hello_imgui_api.md
            )")
        },
    };

}


std::vector<Library> imguiManualLibrary()
{
    return
    {
        {
            "imgui_manual", "", "https://github.com/pthom/imgui_manual",
             "Dear ImGui Manual: an interactive manual for ImGui",
             make_string_vec(R"(
                Readme.md
                LICENSE
                ImGuiManual.cpp
                diagram.png
                diagram.md
                Sources.h
                ImGuiDemoBrowser.h
                ImGuiDemoBrowser.cpp
            )")
        },
    };
}

std::vector<Library> acknowldegmentLibraries()
{
    auto r = fplus::append(otherLibraries(), helloImGuiLibrary());
    return r;
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

    static std::string regionToken = "DemoCode";

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
