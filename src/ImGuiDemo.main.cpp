#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
#include "ImGuiExt.h"
#include "TextEditor.h"
#include "LibrarySources.h"
#include "MarkdownHelper.h"
#include "HyperlinkHelper.h"
#include <fplus/fplus.hpp>



struct AppState
{
    AnnotatedSourceCode annotatedSourceCode = ReadSelectedLibrarySource("imgui/imgui_demo.cpp");
    std::vector<LibrarySources> librarySources = thisLibrarySources();
    bool showAllLibraries = false;
    bool showThisLibrarySources = false;
    bool openSourceCodeInBrowser = false;
    TextEditor editor;
};
AppState gAppState;

void DemoCallback(int line_number)
{
    if (fplus::is_suffix_of(std::string("imgui_demo.cpp"), gAppState.annotatedSourceCode.sourcePath))
    {
        if (gAppState.openSourceCodeInBrowser)
        {
            std::string url = "https://github.com/ocornut/imgui/blob/docking/imgui_demo.cpp#L"
              + std::to_string(line_number);
            HyperlinkHelper::OpenUrl(url);
        }
        gAppState.editor.SetCursorPosition({line_number, 0});
    }
}


bool guiSelectLibrarySource(
        const std::vector<LibrarySources>& librarySources,
        AnnotatedSourceCode *selectedLibrarySource)
{
    bool changed = false;
    ImGui::Checkbox("Show code links in external browser", &gAppState.openSourceCodeInBrowser);
    ImGui::SameLine();
    ImGui::Checkbox("All sources", &gAppState.showThisLibrarySources);
    if (!gAppState.showThisLibrarySources)
        return false;
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
            ImGuiExt::SameLine_IfPossible(400.f);
        }
        ImGui::SameLine(ImGui::GetIO().DisplaySize.x - 350.f );
        ImGuiExt::Hyperlink(librarySource.url);
    }
    return changed;
}


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
    if (ImGui::Checkbox("Other libraries sources", &appState.showAllLibraries))
    {
        if (appState.showAllLibraries)
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
    auto & appState = gAppState;
    appState.editor.SetPalette(TextEditor::GetLightPalette());

    auto lang = TextEditor::LanguageDefinition::CPlusPlus();
    appState.editor.SetLanguageDefinition(lang);

    auto SetupEditor = [&appState]() {
        setEditorAnnotatedSource(appState.annotatedSourceCode, appState.editor);
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
        { "MainDockSpace", "CodeSpace", ImGuiDir_Right, 0.5 },
    };

    // Dockable windows definitions
    HelloImGui::DockableWindow implotDock;
    {
        implotDock.label = "Dear ImGui Demo";
        implotDock.dockSpaceName = "CodeSpace";
        implotDock.GuiFonction = [&implotDock] {
            if (implotDock.isVisible)
                ImGui::ShowDemoWindow(nullptr);
        };
        implotDock.callBeginEnd = false;
    };

    HelloImGui::DockableWindow codeDock;
    {
        codeDock.label = "Code";
        //codeDock.dockSpaceName = "CodeSpace";
        codeDock.dockSpaceName = "MainDockSpace";
        codeDock.GuiFonction = [&appState,&SetupEditor] {
            if (guiSelectLibrarySource(appState.librarySources, &(appState.annotatedSourceCode)))
                SetupEditor();
            guiSourceCategories(appState);
            guiCodeRegions(appState.annotatedSourceCode.linesWithNotes, appState.editor);

            if (fplus::is_suffix_of(std::string(".md"), appState.annotatedSourceCode.sourcePath))
                MarkdownHelper::Markdown(appState.annotatedSourceCode.sourceCode);
            else
                appState.editor.Render(appState.annotatedSourceCode.sourcePath.c_str());
        };
    }

    // Menu
    runnerParams.callbacks.ShowMenus = [&appState]() {
        menuEditorTheme(appState.editor);
    };

    // Fonts
    runnerParams.callbacks.LoadAdditionalFonts = MarkdownHelper::LoadFonts;

    // Set app dockable windows
    runnerParams.dockingParams.dockableWindows = { implotDock, codeDock };

    gImGuiDemoCallback = DemoCallback;

    HelloImGui::Run(runnerParams);
    return 0;
}
