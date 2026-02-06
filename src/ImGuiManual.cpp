#include "ImGuiHeaderDocBrowser.h"
#include "ImGuiDemoBrowser.h"
#include "imgui_utilities/HyperlinkHelper.h"
#include "hello_imgui/hello_imgui.h"
#include "imgui_utilities/MarkdownHelper.h"

HelloImGui::RunnerParams runnerParams;

int main(int, char **)
{
    // Our gui providers for the different windows
    ImGuiDemoBrowser imGuiDemoBrowser;
    ImGuiHeaderDocBrowser imGuiHeaderDocBrowser;

    //
    // Below, we will define all our application parameters and callbacks
    // before starting it.
    //
    runnerParams.imGuiWindowParams.tweakedTheme.Theme = ImGuiTheme::ImGuiTheme_DarculaDarker;
    //runnerParams.imGuiWindowParams.tweakedTheme.Theme = ImGuiTheme::ImGuiTheme_ImGuiColorsDark;

    // App window params
    runnerParams.appWindowParams.windowTitle = "Dear ImGui Manual";
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
            dock_imguiDemoWindow.includeInViewMenu = false;
        };

        HelloImGui::DockableWindow dock_imguiDemoCode;
        {
            dock_imguiDemoCode.label = "Demo Code";
            dock_imguiDemoCode.dockSpaceName = "CodeSpace";// This window goes into "CodeSpace"
            dock_imguiDemoCode.isVisible = true;
            dock_imguiDemoCode.GuiFunction = [&imGuiDemoBrowser] { imGuiDemoBrowser.gui(); };
            dock_imguiDemoCode.imGuiWindowFlags = ImGuiWindowFlags_HorizontalScrollbar;
            dock_imguiDemoCode.includeInViewMenu = false;
        };

        HelloImGui::DockableWindow dock_imGuiHeaderDocBrowser;
        {
            dock_imGuiHeaderDocBrowser.label = imGuiHeaderDocBrowser.windowLabel();
            dock_imGuiHeaderDocBrowser.dockSpaceName = "CodeSpace";
            dock_imGuiHeaderDocBrowser.isVisible = false;
            dock_imGuiHeaderDocBrowser.GuiFunction = [&imGuiHeaderDocBrowser] { imGuiHeaderDocBrowser.gui(); };
        };

        //
        // Set our app dockable windows list
        //
        runnerParams.dockingParams.dockableWindows = {
            dock_imguiDemoCode,
            dock_imguiDemoWindow,
            dock_imGuiHeaderDocBrowser,
        };
    }

    // Set the app menu
    runnerParams.callbacks.ShowMenus = []{
        HelloImGui::DockableWindow *aboutWindow =
            runnerParams.dockingParams.dockableWindowOfName("About this manual");
          HelloImGui::DockableWindow *acknowledgmentWindow =
              runnerParams.dockingParams.dockableWindowOfName("Acknowledgments");
        if (ImGui::BeginMenu("Links & About"))
        {
            ImGui::SeparatorText("Links");
            if (ImGui::MenuItem("Dear ImGui - Github repository"))
                HyperlinkHelper::OpenUrl("https://github.com/ocornut/imgui");
            if (ImGui::MenuItem("Dear ImGui - FAQ"))
                HyperlinkHelper::OpenUrl("https://github.com/ocornut/imgui/blob/master/docs/FAQ.md");
            if (ImGui::MenuItem("dearimgui.com"))
                HyperlinkHelper::OpenUrl("https://www.dearimgui.com/");
            ImGui::SetItemTooltip("The exciting placeholder webpage! - Features Dear ImGui Logo");

            // Not up to date anymore (2025-09), misses info about v1.92
            // if (ImGui::MenuItem("imgui-docs: nice third party ImGui Documentation"))
            //     HyperlinkHelper::OpenUrl("https://possiblyashrub.github.io/imgui-docs/");

            ImGui::SeparatorText("Wiki");
            if (ImGui::MenuItem("Dear ImGui - wiki"))
                HyperlinkHelper::OpenUrl("https://github.com/ocornut/imgui/wiki");
            if (ImGui::MenuItem("Bindings"))
                HyperlinkHelper::OpenUrl("https://github.com/ocornut/imgui/wiki/Bindings");
            if (ImGui::MenuItem("Useful Extensions"))
                HyperlinkHelper::OpenUrl("https://github.com/ocornut/imgui/wiki/Useful-Extensions");


            ImGui::SeparatorText("About this manual");
            if (ImGui::MenuItem("Repository"))
                HyperlinkHelper::OpenUrl("https://github.com/pthom/imgui_manual");
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

      // #ifdef __EMSCRIPTEN__
      // JsClipboard_InstallPlaformSetClipboardText();
      // #endif
    };

    runnerParams.dockingParams.focusDockableWindow("Demo Code");

    // #ifdef IMGUIMANUAL_CLIPBOARD_IMPORT_FROM_BROWSER
    //     JsClipboard_AddJsHook();
    // #endif

    HelloImGui::Run(runnerParams);
    return 0;
}
