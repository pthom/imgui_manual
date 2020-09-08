#include "AboutWindow.h"
#include "Acknowledgments.h"
#include "ImGuiCodeBrowser.h"
#include "ImGuiCppDocBrowser.h"
#include "ImGuiHeaderDocBrowser.h"
#include "ImGuiDemoBrowser.h"
#include "ImGuiReadmeBrowser.h"
#include "MenuTheme.h"

#include "hello_imgui/hello_imgui.h"
#include "JsClipboardTricks.h"

struct AppContext
{
    HelloImGui::RunnerParams runnerParams;

    // Our gui providers for the different windows
    ImGuiDemoBrowser imGuiDemoBrowser;
    ImGuiCppDocBrowser imGuiCppDocBrowser;
    ImGuiHeaderDocBrowser imGuiHeaderDocBrowser;
    ImGuiCodeBrowser imGuiCodeBrowser;
    ImGuiReadmeBrowser imGuiReadmeBrowser;
    Acknowledgments acknowledgments;
    AboutWindow aboutWindow;
};

AppContext gAppContext;
HelloImGui::RunnerParams * gRunnerParams = &gAppContext.runnerParams;


void prepareAppContext()
{
    //
    // Below, we will define all our application parameters and callbacks
    // before starting it.
    //

    // App window params
    gRunnerParams->appWindowParams.windowTitle = "ImGui Manual";
    gRunnerParams->appWindowParams.windowSize = { 1200, 800};

    // ImGui window params
    gRunnerParams->imGuiWindowParams.defaultImGuiWindowType =
            HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
    gRunnerParams->imGuiWindowParams.showMenuBar = true;
    gRunnerParams->imGuiWindowParams.showStatusBar = true;

    // Split the screen in two parts (two "DockSpaces")
    // This will split the preexisting default dockspace "MainDockSpace"
    // in two parts:
    // "MainDockSpace" will be on the left, "CodeSpace" will be on the right
    // and occupy 65% of the app width.
    gRunnerParams->dockingParams.dockingSplits = {
        { "MainDockSpace", "CodeSpace", ImGuiDir_Right, 0.65f },
    };

    //
    // Dockable windows definitions
    //
    {
        HelloImGui::DockableWindow dock_imguiDemoWindow;
        {
            dock_imguiDemoWindow.label = "Dear ImGui Demo";
            dock_imguiDemoWindow.dockSpaceName = "MainDockSpace";// This window goes into "MainDockSpace"
            dock_imguiDemoWindow.GuiFonction = [&dock_imguiDemoWindow] {
                bool isVisible = gRunnerParams->dockingParams.dockableWindowOfName("Dear ImGui Demo")->isVisible;
                if (isVisible)
                    ImGui::ShowDemoWindow(nullptr);
            };
            dock_imguiDemoWindow.callBeginEnd = false;
        };

        HelloImGui::DockableWindow dock_imguiDemoCode;
        {
            dock_imguiDemoCode.label = gAppContext.imGuiDemoBrowser.windowLabel();
            dock_imguiDemoCode.dockSpaceName = "CodeSpace";// This window goes into "CodeSpace"
            dock_imguiDemoCode.isVisible = true;
            dock_imguiDemoCode.GuiFonction = [] { gAppContext.imGuiDemoBrowser.gui(); };
            dock_imguiDemoCode.imGuiWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
        };

        HelloImGui::DockableWindow dock_imGuiCppDocBrowser;
        {
            dock_imGuiCppDocBrowser.label = gAppContext.imGuiCppDocBrowser.windowLabel();
            dock_imGuiCppDocBrowser.dockSpaceName = "CodeSpace";
            dock_imGuiCppDocBrowser.isVisible = false;
            dock_imGuiCppDocBrowser.GuiFonction = [] { gAppContext.imGuiCppDocBrowser.gui(); };
        };

        HelloImGui::DockableWindow dock_imGuiHeaderDocBrowser;
        {
            dock_imGuiHeaderDocBrowser.label = gAppContext.imGuiHeaderDocBrowser.windowLabel();
            dock_imGuiHeaderDocBrowser.dockSpaceName = "CodeSpace";
            dock_imGuiHeaderDocBrowser.isVisible = true;
            dock_imGuiHeaderDocBrowser.GuiFonction = [] { gAppContext.imGuiHeaderDocBrowser.gui(); };
        };

        HelloImGui::DockableWindow dock_imguiReadme;
        {
            dock_imguiReadme.label = "ImGui - Readme";
            dock_imguiReadme.dockSpaceName = "CodeSpace";
            dock_imguiReadme.isVisible = false;
            dock_imguiReadme.GuiFonction = [] { gAppContext.imGuiReadmeBrowser.gui(); };
        };

        HelloImGui::DockableWindow dock_imguiCodeBrowser;
        {
            dock_imguiCodeBrowser.label = "ImGui - Code";
            dock_imguiCodeBrowser.dockSpaceName = "CodeSpace";
            dock_imguiCodeBrowser.isVisible = false;
            dock_imguiCodeBrowser.GuiFonction = [] { gAppContext.imGuiCodeBrowser.gui(); };
        };

        HelloImGui::DockableWindow dock_acknowledgments;
        {
            dock_acknowledgments.label = "Acknowledgments";
            dock_acknowledgments.dockSpaceName = "CodeSpace";
            dock_acknowledgments.isVisible = false;
            dock_acknowledgments.includeInViewMenu = false;
            dock_acknowledgments.GuiFonction = [] { gAppContext.acknowledgments.gui(); };
        };

        HelloImGui::DockableWindow dock_about;
        {
            dock_about.label = "About this manual";
            dock_about.dockSpaceName = "CodeSpace";
            dock_about.isVisible = false;
            dock_about.includeInViewMenu = false;
            dock_about.GuiFonction = [] { gAppContext.aboutWindow.gui(); };
        };

        //
        // Set our app dockable windows list
        //
        gRunnerParams->dockingParams.dockableWindows = {
            dock_imguiDemoCode,
            dock_imguiDemoWindow,
            dock_imGuiHeaderDocBrowser,
            dock_imGuiCppDocBrowser,
            // dock_imguiReadme,
            dock_imguiCodeBrowser,
            dock_acknowledgments,
            dock_about};
    }

    // Set the app menu
    gRunnerParams->callbacks.ShowMenus = []{
        menuTheme();

        HelloImGui::DockableWindow *aboutWindow =
            gRunnerParams->dockingParams.dockableWindowOfName("About this manual");
          HelloImGui::DockableWindow *acknowledgmentWindow =
              gRunnerParams->dockingParams.dockableWindowOfName("Acknowledgments");
        if (aboutWindow && ImGui::BeginMenu("About"))
        {
            if (ImGui::MenuItem("About this manual"))
                aboutWindow->isVisible = true;
            if (ImGui::MenuItem("Acknowledgments"))
                acknowledgmentWindow->isVisible = true;
            ImGui::EndMenu();
        }
    };

    // Add some widgets in the status bar
    gRunnerParams->callbacks.ShowStatus = [] {
        MarkdownHelper::Markdown("Dear ImGui Manual - [Repository](https://github.com/pthom/imgui_manual)");
    };

    // Set the custom fonts
    gRunnerParams->callbacks.LoadAdditionalFonts = []() {
      HelloImGui::ImGuiDefaultSettings::LoadDefaultFont_WithFontAwesomeIcons();
      LoadMonospaceFont();
      MarkdownHelper::LoadFonts();
    };

#ifdef IMGUIMANUAL_CLIPBOARD_IMPORT_FROM_BROWSER
    JsClipboard_AddJsHook();
#endif
}


#ifdef HELLOIMGUI_USE_SOKOL

#include "hello_imgui/sokol/hello_imgui_sokol.h"

sapp_desc sokol_main(int argc, char* argv[])
{
    prepareAppContext();
    return HelloImGui::Sokol::make_sapp_desc(gAppContext.runnerParams);
}

#else

int main(int, char **)
{
    prepareAppContext();
    HelloImGui::Run(gAppContext.runnerParams);
    return 0;
}

#endif
