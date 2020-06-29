#include "hello_imgui/hello_imgui_assets.h"
#include <fplus/fplus.hpp>
#include "LibrarySources.h"


std::vector<LibrarySources> thisLibrarySources()
{
    return
    {
        {
            "implot", "ImPlot", "https://github.com/epezent/implot",
                    {
                            "README.md",
                            "implot_demo.cpp",
                            "implot.h",
                            "implot.cpp",
                            "LICENSE",
                    }
        }
    };
}

std::vector<LibrarySources> otherSources()
{
    return
    {
            {
                    "ImGuiColorTextEdit", "ImGuiColorTextEdit", "https://github.com/BalazsJako/ImGuiColorTextEdit",
                    {
                            "README.md",
                            "TextEditor.h",
                            "TextEditor.cpp",
                            "LICENSE",
                            "CONTRIBUTING",
                    }
            },
            {
                    ".", "This Demo", "",
                    {
                            "CMakeLists.txt",
                            "demo.cpp",
                    }
            }
    };
}


std::vector<LibrarySources> allSources()
{
    return fplus::append(thisLibrarySources(), otherSources());
}


LinesWithNotes findCollapsingHeaderRegions(const std::string &sourceCode)
{
    LinesWithNotes r;

    static std::string regionToken = "ImGui::CollapsingHeader";

    auto extractCollapsingHeaderName = [](const std::string &codeLine) {
        // if codeLine == "if (ImGui::CollapsingHeader("Line Plots")) {"
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
            r[(int)(line_number + 1)] = extractCollapsingHeaderName(line);
    }
    return r;
}

AnnotatedSourceCode ReadSelectedLibrarySource(const std::string sourcePath)
{
    std::string assetPath = std::string("code/") + sourcePath;
    auto assetData = HelloImGui::LoadAssetFileData(assetPath.c_str());
    assert(assetData.data != nullptr);

    AnnotatedSourceCode r;
    r.sourcePath = sourcePath;
    r.sourceCode = std::string((const char *) assetData.data);
    r.linesWithNotes = findCollapsingHeaderRegions(r.sourceCode);
    HelloImGui::FreeAssetFileData(&assetData);
    return r;
}
