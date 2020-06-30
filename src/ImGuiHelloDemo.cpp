#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
#include "ImGuiExt.h"
#include "TextEditor.h"
#include "LibrarySources.h"
#include "MarkdownHelper.h"
#include "HyperlinkHelper.h"
#include <fplus/fplus.hpp>

static std::string gImGuiRepoUrl = "https://github.com/pthom/imgui/blob/DemoCode/imgui_demo.cpp";

TextEditor *gEditorImGuiDemo = nullptr;
void implImGuiDemoCallbackDemoCallback(int line_number)
{
    int cursorLineOnPage = 3;
    gEditorImGuiDemo->SetCursorPosition({line_number, 0}, cursorLineOnPage);
}


void menuEditorTheme(const std::vector<TextEditor *> editors)
{
    if (ImGui::BeginMenu("Editor"))
    {
        if (ImGui::MenuItem("Dark palette"))
            for (auto editor: editors)
                editor->SetPalette(TextEditor::GetDarkPalette());
        if (ImGui::MenuItem("Light palette"))
            for (auto editor: editors)
                editor->SetPalette(TextEditor::GetLightPalette());
        if (ImGui::MenuItem("Retro blue palette"))
            for (auto editor: editors)
                editor->SetPalette(TextEditor::GetRetroBluePalette());
        ImGui::EndMenu();
    }
}


TextEditor factorCppEditor()
{
    TextEditor editor;
    editor.SetPalette(TextEditor::GetLightPalette());
    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
    return editor;
}


class LibrariesCodeBrowser
{
public:
    LibrariesCodeBrowser(
            const std::vector<LibrarySources>& librarySources,
            std::string currentSourcePath
            ) :
              mLibrarySources(librarySources)
            , mCurrentSource(ReadSource(currentSourcePath))
            , mEditor(factorCppEditor())
    {
        mEditor.SetText(mCurrentSource.sourceCode);
    }

    void gui()
    {
        if (guiSelectLibrarySource())
            mEditor.SetText(mCurrentSource.sourceCode);

        if (fplus::is_suffix_of(std::string(".md"), mCurrentSource.sourcePath))
            MarkdownHelper::Markdown(mCurrentSource.sourceCode);
        else
            mEditor.Render(mCurrentSource.sourcePath.c_str());
    }
private:
    bool guiSelectLibrarySource()
    {
        bool changed = false;
        for (const auto & librarySource: mLibrarySources)
        {
            ImGui::Text("%s", librarySource.name.c_str());
            ImGui::SameLine(ImGui::GetWindowSize().x - 350.f );
            ImGuiExt::Hyperlink(librarySource.url);
            for (const auto & source: librarySource.sources)
            {
                std::string currentSourcePath = librarySource.path + "/" + source;
                bool isSelected = (currentSourcePath == mCurrentSource.sourcePath);
                std::string buttonLabel = source + "##" + librarySource.path;
                if (isSelected)
                    ImGui::TextDisabled("%s", source.c_str());
                else if (ImGui::Button(buttonLabel.c_str()))
                {
                    mCurrentSource = ReadSource(currentSourcePath);
                    changed = true;
                }
                ImGuiExt::SameLine_IfPossible(80.f);
            }
            ImGui::NewLine();
            ImGui::Separator();
        }
        return changed;
    }

private:
    std::vector<LibrarySources> mLibrarySources;
    Source mCurrentSource;
    TextEditor mEditor;
};


class ImGuiDemoBrowser
{
public:
    ImGuiDemoBrowser() :
          mAnnotatedSource(ReadAnnotatedSource("imgui/imgui_demo.cpp"))
        , mEditor(factorCppEditor())
    {
        setEditorAnnotatedSource();
    }

    void gui()
    {
        std::string help =
"This is the code of imgui_demo.cpp. It is the best way to learn about Dear ImGui! \n"
"On the left, you can see a demo that showcases all the widgets and features of ImGui: "
"Click on the \"Code\" buttons to see their code and learn about them. \n"
"Alternatively, you can also search for some features (try searching for \"widgets\", \"layout\", \"drag\", etc)";
        ImGui::TextWrapped("%s", help.c_str());
        guiDemoCodeTags();
        guiGithubButton();
        mEditor.Render("imgui_demo.cpp");
    }

    TextEditor * _GetTextEditorPtr() {
        return &mEditor;
    }
private:
    void guiGithubButton()
    {
        if (ImGui::Button("View on github at this line"))
        {
            std::string url = gImGuiRepoUrl + "#L"
                              + std::to_string(mEditor.GetCursorPosition().mLine);
            HyperlinkHelper::OpenUrl(url);
        }
    }


    void guiDemoCodeTags()
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
            for (const auto & lineWithNote : mAnnotatedSource.linesWithNotes)
            {
                if (filter.PassFilter(lineWithNote.second.c_str()))
                {
                    if (ImGui::Button(lineWithNote.second.c_str()))
                        mEditor.SetCursorPosition({lineWithNote.first, 0}, 3);
                    ImGuiExt::SameLine_IfPossible();
                }
            }
            ImGui::NewLine();
        }
    }

    void setEditorAnnotatedSource()
    {
        mEditor.SetText(mAnnotatedSource.source.sourceCode);
        std::unordered_set<int> lineNumbers;
        for (auto kv : mAnnotatedSource.linesWithNotes)
            lineNumbers.insert(kv.first);
        mEditor.SetBreakpoints(lineNumbers);
    }


    AnnotatedSourceCode mAnnotatedSource;
    TextEditor mEditor;
};


// Show ImGui Readme.md
class ImGuiReadmeBrowser
{
public:
    ImGuiReadmeBrowser() : mSource(ReadSource("imgui/README.md")) {}
    void gui() {
        MarkdownHelper::Markdown(mSource.sourceCode);
    }
private:
    Source mSource;
};

// Show doc in imgui.cpp
class ImGuiCppDocBrowser
{
public:
    ImGuiCppDocBrowser();
    void gui()
    {

    }
private:
    Source mSource;
    TextEditor mEditor;
};

struct AppState
{
    std::map<std::string, LibrariesCodeBrowser> librariesCodeBrowsers =
        {
            { "imguiSources", {imguiSources(), "imgui/imgui.cpp"} },
            { "thisDemoSources", {thisDemoSources(), "imgui_hellodemo/Readme.md"} },
            { "otherLibrariesSources", {otherLibrariesSources(), ""} },
        };
};



int main(int, char **)
{
    ImGuiDemoBrowser imGuiDemoBrowser;

    gEditorImGuiDemo = imGuiDemoBrowser._GetTextEditorPtr();

    std::vector<TextEditor *> allEditors {
        imGuiDemoBrowser._GetTextEditorPtr()
    };

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
        dock_imguiDemoCode.GuiFonction = [&imGuiDemoBrowser]{ imGuiDemoBrowser.gui(); };
    };

    HelloImGui::DockableWindow dock_imguiReadme;
    {
        dock_imguiReadme.label = "ImGui - Readme";
        dock_imguiReadme.dockSpaceName = "CodeSpace";
        dock_imguiReadme.GuiFonction = []{
            static ImGuiReadmeBrowser w;
            w.gui();
        };
    };


    // Menu
    runnerParams.callbacks.ShowMenus = [&allEditors]() {
        menuEditorTheme(allEditors);
    };

    // Fonts
    runnerParams.callbacks.LoadAdditionalFonts = MarkdownHelper::LoadFonts;

    // Set app dockable windows
    runnerParams.dockingParams.dockableWindows = {
            dock_imguiDemoCode,
            dock_imguiDemoWindow,
            dock_imguiReadme
            //dock_code,
    };

    gImGuiDemoCallback = implImGuiDemoCallbackDemoCallback;

    HelloImGui::Run(runnerParams);
    return 0;
}
