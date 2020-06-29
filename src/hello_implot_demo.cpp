#include "hello_imgui/hello_imgui.h"
#include "implot.h"
#include "ImGuiExt.h"
#include "TextEditor.h"
#include "LibrarySources.h"


bool guiSelectLibrarySource(
        const std::vector<LibrarySources>& librarySources,
        AnnotatedSourceCode *selectedLibrarySource)
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
        ImGuiExt::Hyperlink(librarySource.url);
    }
    return changed;
}

struct AppState
{
    AnnotatedSourceCode annotatedSourceCode = ReadSelectedLibrarySource("implot/implot_demo.cpp");
    std::vector<LibrarySources> librarySources = thisLibrarySources();
    bool showAllSources = false;
};

void guiSourceCategories(AppState &appState);


void menuEditorTheme(TextEditor &editor) {
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
}

void guiSourceCategories(AppState &appState) {
    if (ImGui::Checkbox("Show other sources", &appState.showAllSources))
    {
        if (appState.showAllSources)
            appState.librarySources = allSources();
        else
            appState.librarySources = thisLibrarySources();
    }
}


void guiCodeRegions(const LinesWithNotes &linesWithNotes, TextEditor &editor)
{
    for (const auto & lineWithNote : linesWithNotes) {
        if (ImGui::Button(lineWithNote.second.c_str()))
            editor.SetCursorPosition({lineWithNote.first, 0});
        ImGuiExt::SameLine_IfPossible();
    }
    ImGui::NewLine();
}

void setEditorAnnotatedSource(const AnnotatedSourceCode& annotatedSourceCode, TextEditor& editor)
{
    editor.SetText(annotatedSourceCode.sourceCode);
    std::unordered_set<int> lineNumbers;
    for (auto kv : annotatedSourceCode.linesWithNotes)
        lineNumbers.insert(kv.first);
    editor.SetBreakpoints(lineNumbers);
}


int main(int, char **)
{
    AppState appState;

    TextEditor editor;
    editor.SetPalette(TextEditor::GetLightPalette());

    auto lang = TextEditor::LanguageDefinition::CPlusPlus();
    editor.SetLanguageDefinition(lang);

    auto SetupEditor = [&editor, &appState]() {
        setEditorAnnotatedSource(appState.annotatedSourceCode, editor);
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
            if (guiSelectLibrarySource(appState.librarySources, &(appState.annotatedSourceCode)))
                SetupEditor();
            guiSourceCategories(appState);
            guiCodeRegions(appState.annotatedSourceCode.linesWithNotes, editor);
            editor.Render(appState.annotatedSourceCode.sourcePath.c_str());
        };
    }

    //
    runnerParams.callbacks.ShowMenus = [&editor]() {
        menuEditorTheme(editor);
    };

    // Set app dockable windows
    runnerParams.dockingParams.dockableWindows = { implotDock, codeDock };


    HelloImGui::Run(runnerParams);
    return 0;
}



