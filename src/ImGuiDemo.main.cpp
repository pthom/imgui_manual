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
    AnnotatedSourceCode otherLibsSourceCode = ReadAnnotatedSource("imgui/README.md");
    std::vector<LibrarySources> librarySources = thisLibrarySources();
    bool showAllLibraries = false;
    LinesWithNotes imguiDemoLinesWithNote;
    TextEditor editorOtherLibs;
    TextEditor editorImGuiDemo;
};
AppState gAppState;



void DemoCallback(int line_number)
{
    int cursorLineOnPage = 3;
    gAppState.editorImGuiDemo.SetCursorPosition({line_number, 0}, cursorLineOnPage);
}


bool guiSelectLibrarySource(
        const std::vector<LibrarySources>& librarySources,
        AnnotatedSourceCode *selectedLibrarySource)
{
    bool changed = false;
    for (const auto & librarySource: librarySources)
    {
        ImGui::Text("%s", librarySource.name.c_str());
        ImGui::SameLine(ImGui::GetWindowSize().x - 350.f );
        ImGuiExt::Hyperlink(librarySource.url);
        for (const auto & source: librarySource.sources)
        {
            std::string currentSourcePath = librarySource.path + "/" + source;
            bool isSelected = (currentSourcePath == selectedLibrarySource->sourcePath);
            std::string buttonLabel = source + "##" + librarySource.path;
            if (isSelected)
                ImGui::TextDisabled("%s", source.c_str());
            else if (ImGui::Button(buttonLabel.c_str()))
            {
                *selectedLibrarySource = ReadAnnotatedSource(currentSourcePath);
                changed = true;
            }
            ImGuiExt::SameLine_IfPossible(80.f);
        }
        ImGui::NewLine();
        ImGui::Separator();
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


void guiSourceCategories(AppState &appState)
{
    if (ImGui::Checkbox("Other libraries sources", &appState.showAllLibraries))
    {
        if (appState.showAllLibraries)
            appState.librarySources = allSources();
        else
            appState.librarySources = thisLibrarySources();
    }
}


void guiDemoCodeRegions(const LinesWithNotes &linesWithNotes, TextEditor &editor)
{
    bool showTooltip = false;
    ImGui::Text("Search demos"); ImGui::SameLine();
    if (ImGui::IsItemHovered())
        showTooltip = true;
    ImGui::TextDisabled("?"); ImGui::SameLine();
    if (ImGui::IsItemHovered())
        showTooltip = true;
    if (showTooltip)
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted("Filter usage:\n"
                               "  \"\"         display all\n"
                               "  \"xxx\"      display items containing \"xxx\"\n"
                               "  \"xxx,yyy\"  display items containing \"xxx\" or \"yyy\"\n"
                               "  \"-xxx\"     hide items containing \"xxx\"");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }


    static ImGuiTextFilter filter;
    filter.Draw();
    if (strlen(filter.InputBuf) >= 3)
    {
        for (const auto & lineWithNote : linesWithNotes)
        {
            if (filter.PassFilter(lineWithNote.second.c_str()))
            {
                if (ImGui::Button(lineWithNote.second.c_str()))
                    editor.SetCursorPosition({lineWithNote.first, 0}, 3);
                ImGuiExt::SameLine_IfPossible();
            }
        }
        ImGui::NewLine();
    }
}


void setEditorAnnotatedSource(const AnnotatedSourceCode& annotatedSourceCode, TextEditor& editor)
{
    editor.SetText(annotatedSourceCode.sourceCode);
    std::unordered_set<int> lineNumbers;
    for (auto kv : annotatedSourceCode.linesWithNotes)
        lineNumbers.insert(kv.first);
    editor.SetBreakpoints(lineNumbers);
}

void setupEditorImGuiDemo()
{
    static auto annotatedSourceCode = ReadAnnotatedSource("imgui/imgui_demo.cpp");
    gAppState.imguiDemoLinesWithNote = annotatedSourceCode.linesWithNotes;
    auto & editor = gAppState.editorImGuiDemo;
    editor.SetPalette(TextEditor::GetLightPalette());
    editor.SetText(annotatedSourceCode.sourceCode);
    std::unordered_set<int> lineNumbers;
    for (auto kv : annotatedSourceCode.linesWithNotes)
        lineNumbers.insert(kv.first);
    editor.SetBreakpoints(lineNumbers);
}

void setupEditorOtherLibraries()
{
    setEditorAnnotatedSource(gAppState.otherLibsSourceCode, gAppState.editorOtherLibs);
};

void guiImguiDemoCode()
{
    guiDemoCodeRegions(gAppState.imguiDemoLinesWithNote, gAppState.editorImGuiDemo);

    if (ImGui::Button("View on github at this line"))
    {
        std::string url = "https://github.com/pthom/imgui/blob/DemoCode/imgui_demo.cpp#L"
                          + std::to_string(gAppState.editorImGuiDemo.GetCursorPosition().mLine);
        HyperlinkHelper::OpenUrl(url);
    }

    gAppState.editorImGuiDemo.Render("imgui_demo.cpp");
}

void guiOtherLibrariesCode(AppState& appState)
{
    guiSourceCategories(appState);
    if (guiSelectLibrarySource(appState.librarySources, &(appState.otherLibsSourceCode)))
        setupEditorOtherLibraries();

    if (fplus::is_suffix_of(std::string(".md"), appState.otherLibsSourceCode.sourcePath))
        MarkdownHelper::Markdown(appState.otherLibsSourceCode.sourceCode);
    else
        appState.editorOtherLibs.Render(appState.otherLibsSourceCode.sourcePath.c_str());
}



int main(int, char **)
{
    gAppState.editorOtherLibs.SetPalette(TextEditor::GetLightPalette());

    auto lang = TextEditor::LanguageDefinition::CPlusPlus();
    gAppState.editorOtherLibs.SetLanguageDefinition(lang);
    gAppState.editorImGuiDemo.SetLanguageDefinition(lang);
    setupEditorOtherLibraries();
    setupEditorImGuiDemo();

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
        { "MainDockSpace", "CodeSpace", ImGuiDir_Right, 0.65f },
    };

    // Dockable windows definitions
    HelloImGui::DockableWindow dock_imguiDemoWindow;
    {
        dock_imguiDemoWindow.label = "Dear ImGui Demo";
        dock_imguiDemoWindow.dockSpaceName = "MainDockSpace";
        dock_imguiDemoWindow.GuiFonction = [&dock_imguiDemoWindow] {
            if (dock_imguiDemoWindow.isVisible)
                ImGui::ShowDemoWindow(nullptr);
        };
        dock_imguiDemoWindow.callBeginEnd = false;
    };

    HelloImGui::DockableWindow dock_imguiDemoCode;
    {
        dock_imguiDemoCode.label = "imgui_demo code";
        dock_imguiDemoCode.dockSpaceName = "CodeSpace";
        dock_imguiDemoCode.GuiFonction = guiImguiDemoCode;
    };

    HelloImGui::DockableWindow dock_code;
    {
        dock_code.label = "All sources";
        dock_code.dockSpaceName = "CodeSpace";
        dock_code.GuiFonction = [] { guiOtherLibrariesCode(gAppState); };
    }

    // Menu
    runnerParams.callbacks.ShowMenus = []() {
        menuEditorTheme(gAppState.editorOtherLibs);
    };

    // Fonts
    runnerParams.callbacks.LoadAdditionalFonts = MarkdownHelper::LoadFonts;

    // Set app dockable windows
    runnerParams.dockingParams.dockableWindows = {
            dock_imguiDemoCode,
            dock_imguiDemoWindow,
            dock_code,
    };

    gImGuiDemoCallback = DemoCallback;

    HelloImGui::Run(runnerParams);
    return 0;
}

