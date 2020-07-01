#include "hello_imgui/hello_imgui.h"
#include "imgui.h"
#include "ImGuiExt.h"
#include "TextEditor.h"
#include "WindowWithEditor.h"
#include "Sources.h"
#include "MarkdownHelper.h"
#include "HyperlinkHelper.h"
#include <fplus/fplus.hpp>
#include <functional>



static std::string gImGuiRepoUrl = "https://github.com/pthom/imgui/blob/DemoCode/";

TextEditor *gEditorImGuiDemo = nullptr;

// This is a callback that will be called by imgui_demo.cpp
void implImGuiDemoCallbackDemoCallback(int line_number)
{
    int cursorLineOnPage = 3;
    gEditorImGuiDemo->SetCursorPosition({line_number, 0}, cursorLineOnPage);
}


class LibrariesCodeBrowser: public WindowWithEditor
{
public:
    LibrariesCodeBrowser(
            const std::vector<Sources::Library>& librarySources,
            std::string currentSourcePath
            ) :
              WindowWithEditor()
            , mLibrarySources(librarySources)
            , mCurrentSource(Sources::ReadSource(currentSourcePath))
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
            RenderEditor(mCurrentSource.sourcePath.c_str());
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
            MarkdownHelper::Markdown(librarySource.shortDoc);
            for (const auto & source: librarySource.sourcePaths)
            {
                std::string currentSourcePath = librarySource.path + "/" + source;
                bool isSelected = (currentSourcePath == mCurrentSource.sourcePath);
                std::string buttonLabel = source + "##" + librarySource.path;
                if (isSelected)
                    ImGui::TextDisabled("%s", source.c_str());
                else if (ImGui::Button(buttonLabel.c_str()))
                {
                    mCurrentSource = Sources::ReadSource(currentSourcePath);
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
    std::vector<Sources::Library> mLibrarySources;
    Sources::Source mCurrentSource;
};


class ImGuiDemoBrowser: public WindowWithEditor
{
public:
    ImGuiDemoBrowser() :
            WindowWithEditor()
          , mAnnotatedSource(Sources::ReadImGuiDemoCode("imgui/imgui_demo.cpp"))
    {
        setEditorAnnotatedSource(mAnnotatedSource);
    }

    void gui()
    {
        guiHelp();
        guiDemoCodeTags();
        guiSave();
        RenderEditor("imgui_demo.cpp", [this] { this->guiGithubButton(); });
    }

private:
    void guiHelp()
    {
        static bool showHelp = true;
        if (showHelp)
        {
            std::string help =
                "This is the code of imgui_demo.cpp. It is the best way to learn about Dear ImGui! \n"
                "On the left, you can see a demo that showcases all the widgets and features of ImGui: "
                "Click on the \"Code\" buttons to see their code and learn about them. \n"
                "Alternatively, you can also search for some features (try searching for \"widgets\", \"layout\", \"drag\", etc)";
            ImGui::TextWrapped("%s", help.c_str());
            //ImGui::SameLine();
            if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
                showHelp = false;
        }
    }
    void guiSave()
    {
#ifdef IMGUI_MANUAL_CAN_WRITE_IMGUI_DEMO_CPP
        if (ImGui::Button("Save"))
        {
            std::string fileSrc = IMGUI_MANUAL_REPO_DIR "/external/imgui/imgui_demo.cpp";
            fplus::write_text_file(fileSrc, mEditor.GetText())();
        }
#endif
    }
    void guiGithubButton()
    {
        if (ImGui::SmallButton("View on github at this line"))
        {
            std::string url = gImGuiRepoUrl + "imgui_demo.cpp#L"
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
            ImGui::TextUnformatted(
                "Filter usage:[-excl],incl\n"
                "For example:\n"
                "   \"button\" will search for \"button\"\n"
                "   \"-widget,button\" will search for \"button\" without \"widget\""
                );
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }


        static ImGuiTextFilter filter;
        filter.Draw();
        if (strlen(filter.InputBuf) >= 3)
        {
            for (const auto & lineWithNote : mAnnotatedSource.linesWithTags)
            {
                if (filter.PassFilter(lineWithNote.tag.c_str()))
                {
                    if (ImGui::Button(lineWithNote.tag.c_str()))
                        mEditor.SetCursorPosition({lineWithNote.lineNumber, 0}, 3);
                    ImGuiExt::SameLine_IfPossible();
                }
            }
            ImGui::NewLine();
        }
    }

    Sources::AnnotatedSource mAnnotatedSource;
};


// Show ImGui Readme.md
class ImGuiReadmeBrowser
{
public:
    ImGuiReadmeBrowser() : mSource(Sources::ReadSource("imgui/README.md")) {}
    void gui() {
        MarkdownHelper::Markdown(mSource.sourceCode);
    }
private:
    Sources::Source mSource;
};

// Show doc in imgui.cpp
class ImGuiCppDocBrowser: public WindowWithEditor
{
public:
    ImGuiCppDocBrowser()
        : WindowWithEditor()
        , mAnnotatedSource(Sources::ReadImGuiCppDoc("imgui/imgui.cpp"))
    {
        setEditorAnnotatedSource(mAnnotatedSource);
    }
    void gui()
    {
        ImGui::Text("The doc for Dear ImGui is simply stored inside imgui.cpp");
        guiTags();
        RenderEditor("imgui.cpp", [this] { this->guiGithubButton(); });
    }
private:
    void guiTags()
    {
        for (auto lineWithTag : mAnnotatedSource.linesWithTags)
        {
            // tags are of type H1 or H2, and begin with "H1 " or "H2 " (3 characters)
            std::string title = fplus::drop(3, lineWithTag.tag);
            bool isHeader1 = (fplus::take(3, lineWithTag.tag) == "H1 ");
            if (isHeader1)
            {
                if (ImGuiExt::ClickableText(title.c_str()))
                    mEditor.SetCursorPosition({lineWithTag.lineNumber, 0}, 3);
            }
        }
    }
    void guiGithubButton()
    {
        if (ImGui::SmallButton("View on github at this line"))
        {
            std::string url = gImGuiRepoUrl + "imgui.cpp#L"
                              + std::to_string(mEditor.GetCursorPosition().mLine);
            HyperlinkHelper::OpenUrl(url);
        }
    }

    Sources::AnnotatedSource mAnnotatedSource;
};


class ImGuiCodeBrowser: public WindowWithEditor
{
public:
    ImGuiCodeBrowser()
        : WindowWithEditor()
        , mLibrariesCodeBrowser(Sources::imguiLibrary(), "imgui/imgui.h")
    {

    }
    void gui()
    {
        guiHelp();
        mLibrariesCodeBrowser.gui();
    }
private:
    void guiHelp()
    {
        static bool showHelp = true;
        if (showHelp)
        {
            // Readme
            std::string help = R"(
This is the core of ImGui code.

Usage (extract from [ImGui Readme](https://github.com/ocornut/imgui#usage))

The core of Dear ImGui is self-contained within a few platform-agnostic files which you can easily compile in your application/engine. They are all the files in the root folder of the repository (imgui.cpp, imgui.h, imgui_demo.cpp, imgui_draw.cpp etc.).

No specific build process is required. You can add the .cpp files to your existing project.

You will need a backend to integrate Dear ImGui in your app. The backend passes mouse/keyboard/gamepad inputs and variety of settings to Dear ImGui, and is in charge of rendering the resulting vertices.

Backends for a variety of graphics api and rendering platforms are provided in the [examples/](https://github.com/ocornut/imgui/tree/master/examples) folder, along with example applications. See the [Integration](https://github.com/ocornut/imgui#integration) section of this document for details. You may also create your own backend. Anywhere where you can render textured triangles, you can render Dear ImGui.
)";
            MarkdownHelper::Markdown(help.c_str());
            if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
                showHelp = false;
        }
    }

    LibrariesCodeBrowser mLibrariesCodeBrowser;
};


class Acknowledgments: public WindowWithEditor
{
public:
    Acknowledgments()
        : WindowWithEditor()
        , mLibrariesCodeBrowser(Sources::otherLibraries(), "")
    {

    }
    void gui()
    {
        guiHelp();
        mLibrariesCodeBrowser.gui();
    }
private:
    void guiHelp()
    {
        static bool showHelp = true;
        if (showHelp)
        {
            std::string help = R"(
This interactive manual was developed using [Hello ImGui](https://github.com/pthom/hello_imgui), which provided the emscripten port, as well as the assets and source code embedding.
See also a [related demo for Implot](https://traineq.org/implot_demo/src/implot_demo.html), which also provides code navigation.

This manual uses some great libraries, which are shown below.
)";
            MarkdownHelper::Markdown(help.c_str());
            if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
                showHelp = false;
        }
    }

    LibrariesCodeBrowser mLibrariesCodeBrowser;
};

void menuTheme()
{
    if (ImGui::BeginMenu("Theme"))
    {
        ImGui::MenuItem("App", NULL, false, false);
        if (ImGui::MenuItem("Dark"))
            ImGui::StyleColorsDark();
        if (ImGui::MenuItem("Classic"))
            ImGui::StyleColorsClassic();
        if (ImGui::MenuItem("Light"))
            ImGui::StyleColorsLight();

        ImGui::Separator();
        menuEditorTheme();
        ImGui::Separator();

        if (ImGui::MenuItem("User contributed themes"))
            HyperlinkHelper::OpenUrl("https://github.com/ocornut/imgui/issues/707");

        ImGui::EndMenu();

    }

}


int main(int, char **)
{
    ImGuiDemoBrowser imGuiDemoBrowser;
    ImGuiCppDocBrowser imGuiCppDocBrowser;
    ImGuiCodeBrowser imGuiCodeBrowser;
    Acknowledgments otherLibraries;

    gEditorImGuiDemo = imGuiDemoBrowser._GetTextEditorPtr();

    HelloImGui::RunnerParams runnerParams;

    // App window params
    runnerParams.appWindowParams.windowTitle = "ImGui Manual";
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
        dock_imguiDemoCode.label = "ImGui - Demo Code";
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

    HelloImGui::DockableWindow dock_imGuiCppDocBrowser;
    {
        dock_imGuiCppDocBrowser.label = "ImGui - Doc";
        dock_imGuiCppDocBrowser.dockSpaceName = "CodeSpace";
        dock_imGuiCppDocBrowser.GuiFonction = [&imGuiCppDocBrowser]{ imGuiCppDocBrowser.gui(); };
    };

    HelloImGui::DockableWindow dock_imguiCodeBrowser;
    {
        dock_imguiCodeBrowser.label = "ImGui - Code";
        dock_imguiCodeBrowser.dockSpaceName = "CodeSpace";
        dock_imguiCodeBrowser.GuiFonction = [&imGuiCodeBrowser]{ imGuiCodeBrowser.gui(); };
    };

    HelloImGui::DockableWindow dock_acknowledgments;
    {
        dock_acknowledgments.label = "Acknowledgments";
        dock_acknowledgments.dockSpaceName = "CodeSpace";
        dock_acknowledgments.GuiFonction = [&otherLibraries]{ otherLibraries.gui(); };
    };

    HelloImGui::DockableWindow dock_about;
    {
        dock_about.label = "About this manual";
        dock_about.dockSpaceName = "";
        dock_about.isAboutWindow = true;
        dock_about.isVisible = false;
        dock_about.imGuiWindowFlags = ImGuiWindowFlags_AlwaysAutoResize;
        dock_about.GuiFonction = []{
            ImGui::Text("About...");
        };
    };

    // Menu
    runnerParams.callbacks.ShowMenus = menuTheme;

    // Fonts
    runnerParams.callbacks.LoadAdditionalFonts = MarkdownHelper::LoadFonts;

    // Set app dockable windows
    runnerParams.dockingParams.dockableWindows = {
        dock_imguiDemoCode,
        dock_imguiDemoWindow,
        dock_imguiReadme,
        dock_imGuiCppDocBrowser,
        dock_imguiCodeBrowser,
        dock_acknowledgments,
        dock_about
    };

    gImGuiDemoCallback = implImGuiDemoCallbackDemoCallback;

    HelloImGui::Run(runnerParams);
    return 0;
}
