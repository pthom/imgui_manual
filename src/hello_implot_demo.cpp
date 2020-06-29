#include "hello_imgui/hello_imgui.h"
#include "implot.h"
#include "TextEditor.h"
#include "LibrarySources.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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
        ImGui::SameLine(ImGui::GetIO().DisplaySize.x - 300.f );
#ifdef __EMSCRIPTEN__
        std::string linkLabel = std::string(ICON_FA_LINK) + " ##" + librarySource.url;
        if (ImGui::Button(linkLabel.c_str()))
        {
            std::string js_command = "window.open(\"" + librarySource.url + "\");";
            emscripten_run_script(js_command.c_str());
        }
        ImGui::SameLine();
#endif
        auto blue = ImVec4(0.3f, 0.5f, 1.f, 1.f);
        ImGui::TextColored(blue, "%s", librarySource.url.c_str()); ImGui::SameLine();
        ImGui::NewLine();
    }
    return changed;
}

struct AppState
{
    SelectedLibrarySource selectedLibrarySource = ReadSelectedLibrarySource("implot/implot_demo.cpp");
    std::vector<LibrarySources> librarySources = thisLibrarySources();
    bool showAllSources = false;
};

int main(int, char **)
{
    AppState appState;

    TextEditor editor;
    editor.SetPalette(TextEditor::GetLightPalette());

    auto lang = TextEditor::LanguageDefinition::CPlusPlus();
    editor.SetLanguageDefinition(lang);

    auto SetupEditor = [&editor, &appState]() {
        editor.SetText(appState.selectedLibrarySource.sourceCode);
        std::unordered_set<int> lineNumbers;
        for (auto kv : appState.selectedLibrarySource.linesWithNotes)
            lineNumbers.insert(kv.first);
        editor.SetBreakpoints(lineNumbers);
        //editor.SetErrorMarkers(appState.selectedLibrarySource.markers);
    };

    SetupEditor();

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
        codeDock.GuiFonction = [&editor, &appState,&SetupEditor] {
            if (Gui_SelectLibrarySource(appState.librarySources, &(appState.selectedLibrarySource)))
                SetupEditor();
            if (ImGui::Checkbox("Show other sources", &appState.showAllSources))
            {
                if (appState.showAllSources)
                    appState.librarySources = allSources();
                else
                    appState.librarySources = thisLibrarySources();
            }
            for (const auto & lineWithNote : appState.selectedLibrarySource.linesWithNotes) {
                if (ImGui::Button(lineWithNote.second.c_str()))
                    ;
                ImGui::SameLine();
            }
            ImGui::NewLine();

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