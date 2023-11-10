#include "AboutWindow.h"
#include "Acknowledgments.h"
#include "ImGuiCodeBrowser.h"
#include "ImGuiCppDocBrowser.h"
#include "ImGuiHeaderDocBrowser.h"
#include "ImGuiDemoBrowser.h"
#include "ImGuiReadmeBrowser.h"
#include "imgui_utilities/HyperlinkHelper.h"


#include "hello_imgui/hello_imgui.h"
#include "JsClipboardTricks.h"

HelloImGui::RunnerParams runnerParams;

int main(int, char **)
{
    // Our gui providers for the different windows
    ImGuiDemoBrowser imGuiDemoBrowser;
    ImGuiCppDocBrowser imGuiCppDocBrowser;
    ImGuiHeaderDocBrowser imGuiHeaderDocBrowser;
    ImGuiCodeBrowser imGuiCodeBrowser;
    ImGuiReadmeBrowser imGuiReadmeBrowser;
    Acknowledgments acknowledgments;
    AboutWindow aboutWindow;

    //
    // Below, we will define all our application parameters and callbacks
    // before starting it.
    //
    runnerParams.imGuiWindowParams.tweakedTheme.Theme = ImGuiTheme::ImGuiTheme_DarculaDarker;

    // App window params
    runnerParams.appWindowParams.windowTitle = "ImGui Manual";
    runnerParams.appWindowParams.windowGeometry.size = {1200, 800};

    // ImGui window params
    runnerParams.imGuiWindowParams.defaultImGuiWindowType =
            HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
    runnerParams.imGuiWindowParams.showMenuBar = true;
    runnerParams.imGuiWindowParams.showStatusBar = true;

    // Split the screen in two parts (two "DockSpaces")
    // This will split the preexisting default dockspace "MainDockSpace"
    // in two parts:
    // "MainDockSpace" will be on the left, "CodeSpace" will be on the right
    // and occupy 65% of the app width.
    runnerParams.dockingParams.dockingSplits = {
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
            dock_imguiDemoWindow.GuiFunction = [&dock_imguiDemoWindow] {
                if (dock_imguiDemoWindow.isVisible)
                    ImGui::ShowDemoWindow(nullptr);
            };
            dock_imguiDemoWindow.callBeginEnd = false;
        };

        HelloImGui::DockableWindow dock_imguiDemoCode;
        {
            dock_imguiDemoCode.label = "Demo Code";
            dock_imguiDemoCode.dockSpaceName = "CodeSpace";// This window goes into "CodeSpace"
            dock_imguiDemoCode.isVisible = true;
            dock_imguiDemoCode.GuiFunction = [&imGuiDemoBrowser] { imGuiDemoBrowser.gui(); };
            dock_imguiDemoCode.imGuiWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
        };

        HelloImGui::DockableWindow dock_imGuiCppDocBrowser;
        {
            dock_imGuiCppDocBrowser.label = imGuiCppDocBrowser.windowLabel();
            dock_imGuiCppDocBrowser.dockSpaceName = "CodeSpace";
            dock_imGuiCppDocBrowser.isVisible = false;
            dock_imGuiCppDocBrowser.GuiFunction = [&imGuiCppDocBrowser] { imGuiCppDocBrowser.gui(); };
        };

        HelloImGui::DockableWindow dock_imGuiHeaderDocBrowser;
        {
            dock_imGuiHeaderDocBrowser.label = imGuiHeaderDocBrowser.windowLabel();
            dock_imGuiHeaderDocBrowser.dockSpaceName = "CodeSpace";
            dock_imGuiHeaderDocBrowser.isVisible = true;
            dock_imGuiHeaderDocBrowser.GuiFunction = [&imGuiHeaderDocBrowser] { imGuiHeaderDocBrowser.gui(); };
        };

        HelloImGui::DockableWindow dock_imguiReadme;
        {
            dock_imguiReadme.label = "ImGui - Readme";
            dock_imguiReadme.dockSpaceName = "CodeSpace";
            dock_imguiReadme.isVisible = false;
            dock_imguiReadme.GuiFunction = [&imGuiReadmeBrowser] { imGuiReadmeBrowser.gui(); };
        };

        HelloImGui::DockableWindow dock_imguiCodeBrowser;
        {
            dock_imguiCodeBrowser.label = "ImGui - Code";
            dock_imguiCodeBrowser.dockSpaceName = "CodeSpace";
            dock_imguiCodeBrowser.isVisible = false;
            dock_imguiCodeBrowser.GuiFunction = [&imGuiCodeBrowser] { imGuiCodeBrowser.gui(); };
        };

        HelloImGui::DockableWindow dock_acknowledgments;
        {
            dock_acknowledgments.label = "Acknowledgments";
            dock_acknowledgments.dockSpaceName = "CodeSpace";
            dock_acknowledgments.isVisible = false;
            dock_acknowledgments.includeInViewMenu = false;
            dock_acknowledgments.GuiFunction = [&acknowledgments] { acknowledgments.gui(); };
        };

        HelloImGui::DockableWindow dock_about;
        {
            dock_about.label = "About this manual";
            dock_about.dockSpaceName = "CodeSpace";
            dock_about.isVisible = false;
            dock_about.includeInViewMenu = false;
            dock_about.GuiFunction = [&aboutWindow] { aboutWindow.gui(); };
        };

        //
        // Set our app dockable windows list
        //
        runnerParams.dockingParams.dockableWindows = {
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
    runnerParams.callbacks.ShowMenus = []{
        HelloImGui::DockableWindow *aboutWindow =
            runnerParams.dockingParams.dockableWindowOfName("About this manual");
          HelloImGui::DockableWindow *acknowledgmentWindow =
              runnerParams.dockingParams.dockableWindowOfName("Acknowledgments");
        if (ImGui::BeginMenu("Links & About"))
        {
            ImGui::TextDisabled("Links");
            if (ImGui::MenuItem("ImGui Github repository"))
                HyperlinkHelper::OpenUrl("https://github.com/ocornut/imgui");
            if (ImGui::MenuItem("ImGui wiki"))
                HyperlinkHelper::OpenUrl("https://github.com/ocornut/imgui/wiki");
            if (ImGui::MenuItem("imgui-docs: nice third party ImGui Documentation"))
                HyperlinkHelper::OpenUrl("https://possiblyashrub.github.io/imgui-docs/");

            ImGui::Separator();
            ImGui::TextDisabled("About this manual");
            if (ImGui::MenuItem("About"))
                aboutWindow->isVisible = true;
            if (ImGui::MenuItem("Repository"))
                HyperlinkHelper::OpenUrl("https://github.com/pthom/imgui_manual");
            if (ImGui::MenuItem("Online interactive manual"))
                HyperlinkHelper::OpenUrl("https://pthom.github.io/imgui_manual_online/");
            if (ImGui::MenuItem("Acknowledgments"))
                acknowledgmentWindow->isVisible = true;
            ImGui::EndMenu();
        }
    };

    // Add some widgets in the status bar
    runnerParams.callbacks.ShowStatus = [] {
        //MarkdownHelper::Markdown("Dear ImGui Manual - [Repository](https://github.com/pthom/imgui_manual)");
        MarkdownHelper::Markdown("Dear ImGui Manual, a manual for [Dear ImGui](https://github.com/ocornut/imgui) - Made with [Dear ImGui Bundle](https://github.com/pthom/imgui_bundle/) and [Hello ImGui](https://github.com/pthom/hello_imgui)");
    };

    // Set the custom fonts
    runnerParams.callbacks.LoadAdditionalFonts = []() {
      HelloImGui::ImGuiDefaultSettings::LoadDefaultFont_WithFontAwesomeIcons();
      LoadMonospaceFont();
      MarkdownHelper::LoadFonts();
    };

    runnerParams.dockingParams.focusDockableWindow("Demo Code");

#ifdef IMGUIMANUAL_CLIPBOARD_IMPORT_FROM_BROWSER
    JsClipboard_AddJsHook();
#endif

    HelloImGui::Run(runnerParams);
    return 0;
}
