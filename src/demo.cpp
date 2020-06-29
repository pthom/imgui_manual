#include "hello_imgui/hello_imgui.h"
#include "implot.h"
#include "TextEditor.h"
#include <vector>
#include <string>

struct LibrarySources
{
    std::string path;
    std::string name;
    std::string url;
    std::vector<std::string> sources;
};

std::vector<LibrarySources> gLibrarySources {
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

std::vector<LibrarySources> gOtherSources {
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

std::vector<LibrarySources> gAllSources = []{
    std::vector<LibrarySources> r;
    // c++ equivalent of r = concat(gLibrarySources, gOtherSources) . Sigh...
    r.insert(r.end(), gLibrarySources.begin(), gLibrarySources.end());
    r.insert(r.end(), gOtherSources.begin(), gOtherSources.end());
    return r;
}();

struct SelectedLibrarySource
{
    std::string sourcePath;
    std::string sourceCode;
};

SelectedLibrarySource ReadSelectedLibrarySource(const std::string sourcePath)
{
    std::string assetPath = std::string("code/") + sourcePath;
    auto assetData = HelloImGui::LoadAssetFileData(assetPath.c_str());
    assert(assetData.data != nullptr);

    SelectedLibrarySource r;
    r.sourcePath = sourcePath;
    r.sourceCode = std::string((const char *) assetData.data);
    HelloImGui::FreeAssetFileData(&assetData);
    return r;
}


bool Gui_SelectLibrarySource(
        const std::vector<LibrarySources>& librarySources,
        SelectedLibrarySource *selectedLibrarySource)
{
    bool changed = false;
    for (const auto & librarySource: librarySources)
    {
        ImGui::Text("%s", librarySource.name.c_str());
        ImGui::SameLine(150.f);
        for (const auto & source: librarySource.sources)
        {
            std::string currentSourcePath = librarySource.path + "/" + source;
            bool isSelected = (currentSourcePath == selectedLibrarySource->sourcePath);
            std::string buttonLabel = source + "##" + librarySource.path;
            if (isSelected)
                ImGui::TextDisabled("%s", source.c_str());
            else if (ImGui::Button(buttonLabel.c_str()))
            {
                *selectedLibrarySource = ReadSelectedLibrarySource(currentSourcePath);
                changed = true;
            }
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
    return changed;
}

struct AppState
{
    SelectedLibrarySource selectedLibrarySource = ReadSelectedLibrarySource("implot/implot_demo.cpp");
    std::vector<LibrarySources> librarySources = gLibrarySources;
    bool showAllSources = false;
};

int main(int, char **)
{
    AppState appState;

    TextEditor editor;
    editor.SetPalette(TextEditor::GetLightPalette());

    auto lang = TextEditor::LanguageDefinition::CPlusPlus();
    editor.SetLanguageDefinition(lang);
    editor.SetText(appState.selectedLibrarySource.sourceCode);

    HelloImGui::RunnerParams runnerParams;

    // App window params
    runnerParams.appWindowParams.windowTitle = "implot demo";
    runnerParams.appWindowParams.windowSize = { 1200, 800};

    // ImGui window params
    runnerParams.imGuiWindowParams.defaultImGuiWindowType =
            HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
    runnerParams.imGuiWindowParams.showMenuBar = true;
    runnerParams.imGuiWindowParams.showStatusBar = true;

    // Split the screen in two parts
    runnerParams.dockingParams.dockingSplits = {
        { "MainDockSpace", "CodeSpace", ImGuiDir_Up, 0.5 },
    };

    // Dockable windows definitions
    HelloImGui::DockableWindow implotDock;
    {
        implotDock.label = "ImPlot Demo";
        implotDock.dockSpaceName = "MainDockSpace";
        implotDock.GuiFonction = [&implotDock] {
            if (implotDock.isVisible)
                ImPlot::ShowDemoWindow(nullptr);
        };
        implotDock.callBeginEnd = false;
    };

    HelloImGui::DockableWindow codeDock;
    {
        codeDock.label = "Code";
        codeDock.dockSpaceName = "CodeSpace";
        codeDock.GuiFonction = [&editor, &appState] {
            if (Gui_SelectLibrarySource(appState.librarySources, &(appState.selectedLibrarySource)))
                editor.SetText(appState.selectedLibrarySource.sourceCode);
            if (ImGui::Checkbox("Show other sources", &appState.showAllSources))
            {
                if (appState.showAllSources)
                    appState.librarySources = gAllSources;
                else
                    appState.librarySources = gLibrarySources;
            }
            editor.Render(appState.selectedLibrarySource.sourcePath.c_str());
        };
    }

    //
    runnerParams.callbacks.ShowMenus = [&editor]() {
        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("Dark palette"))
                editor.SetPalette(TextEditor::GetDarkPalette());
            if (ImGui::MenuItem("Light palette"))
                editor.SetPalette(TextEditor::GetLightPalette());
            if (ImGui::MenuItem("Retro blue palette"))
                editor.SetPalette(TextEditor::GetRetroBluePalette());
            ImGui::EndMenu();
        }
    };

    // Set app dockable windows
    runnerParams.dockingParams.dockableWindows = { implotDock, codeDock };

    HelloImGui::Run(runnerParams);
    return 0;
}