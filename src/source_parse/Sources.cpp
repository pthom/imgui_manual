#include "hello_imgui/hello_imgui_assets.h"
#include <fplus/fplus.hpp>
#include <imgui.h>
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

namespace SourceParse
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
                source_parse/Sources.h
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



} // namespace SourceParse
