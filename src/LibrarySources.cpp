#include "hello_imgui/hello_imgui_assets.h"
#include <fplus/fplus.hpp>
#include "LibrarySources.h"

namespace {
    auto make_string_vec = [](const std::string &s) -> std::vector<std::string> {
        std::vector<std::string> r = fplus::split('\n', false, s);
        r = fplus::transform(fplus::trim_whitespace<std::string>, r);
        r = fplus::keep_if([](auto s) { return !s.empty();}, r);
        return r;
    };
}


std::vector<LibrarySources> thisLibrarySources()
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

std::vector<LibrarySources> otherSources()
{
    return
    {
        {
            "imgui_hellodemo", "This Demo", "https://github.com/pthom/implot_demo",
            make_string_vec(R"(
                Readme.md
                LICENSE
                ImGuiDemo.main.cpp
                LibrarySources.cpp
                LibrarySources.h
                HyperlinkHelper.cpp
                HyperlinkHelper.h
                ImGuiExt.cpp
                ImGuiExt.h
                MarkdownHelper.cpp
                MarkdownHelper.h
                populate_assets.sh
                CMakeLists.txt
        )")
        },
        {
            "hello_imgui", "Hello ImGui", "https://github.com/pthom/hello_imgui",
            make_string_vec(R"(
                README.md
                LICENSE
                hello_imgui.h
                hello_imgui_api.md
                runner_params.h
                app_window_params.h
                imgui_window_params.h
                runner_callbacks.h
                docking_params.h
                hello_imgui_assets.h
                hello_imgui_error.h
                icons_font_awesome.h
                imgui_default_settings.h
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
    };
}


std::vector<LibrarySources> allSources()
{
    return fplus::append(thisLibrarySources(), otherSources());
}


LinesWithNotes findDemoCodeRegions(const std::string &sourceCode)
{
    LinesWithNotes r;

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
            r[(int)(line_number + 1)] = extractDemoCodeName(line);
    }
    return r;
}

AnnotatedSourceCode ReadAnnotatedSource(const std::string sourcePath)
{
    std::string assetPath = std::string("code/") + sourcePath;
    auto assetData = HelloImGui::LoadAssetFileData(assetPath.c_str());
    assert(assetData.data != nullptr);

    AnnotatedSourceCode r;
    r.sourcePath = sourcePath;
    r.sourceCode = std::string((const char *) assetData.data);
    r.linesWithNotes = findDemoCodeRegions(r.sourceCode);
    HelloImGui::FreeAssetFileData(&assetData);
    return r;
}
