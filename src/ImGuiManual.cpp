#include "AboutWindow.h"
#include "Acknowledgments.h"
#include "ImGuiCodeBrowser.h"
#include "ImGuiCppDocBrowser.h"
#include "ImGuiDemoBrowser.h"
#include "ImGuiReadmeBrowser.h"
#include "ImGuiRepoUrl.h"
#include "LibrariesCodeBrowser.h"
#include "MenuTheme.h"
#include "Sources.h"
#include "WindowWithEditor.h"

#include "hello_imgui/hello_imgui.h"


int main(int, char **)
{
    // Our gui providers for the different windows
    ImGuiDemoBrowser imGuiDemoBrowser;
    ImGuiCppDocBrowser imGuiCppDocBrowser;
    ImGuiCodeBrowser imGuiCodeBrowser;
    Acknowledgments acknowledgments;
    AboutWindow aboutWindow;

    //
    // Below, we will define all our application parameters and callbacks
    // before starting it.
    //
    HelloImGui::RunnerParams runnerParams;

    // App window params
    runnerParams.appWindowParams.windowTitle = "ImGui Manual";
    runnerParams.appWindowParams.windowSize = { 1200, 800};

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
    HelloImGui::DockableWindow dock_imguiDemoWindow;
    {
        dock_imguiDemoWindow.label = "Dear ImGui Demo";
        dock_imguiDemoWindow.dockSpaceName = "MainDockSpace"; // This window goes into "MainDockSpace"
        dock_imguiDemoWindow.GuiFonction = [&dock_imguiDemoWindow] {
            if (dock_imguiDemoWindow.isVisible)
                ImGui::ShowDemoWindow(nullptr);
        };
        dock_imguiDemoWindow.callBeginEnd = false;
    };

    HelloImGui::DockableWindow dock_imguiDemoCode;
    {
        dock_imguiDemoCode.label = "ImGui - Demo Code";
        dock_imguiDemoCode.dockSpaceName = "CodeSpace"; // This window goes into "CodeSpace"
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
        dock_acknowledgments.GuiFonction = [&acknowledgments]{ acknowledgments.gui(); };
    };

    HelloImGui::DockableWindow dock_about;
    {
        dock_about.label = "About this manual";
        dock_about.dockSpaceName = "CodeSpace";
        // dock_about.isVisible = false;
        // dock_about.imGuiWindowFlags = ImGuiWindowFlags_AlwaysAutoResize; // it will auto-resize
        dock_about.GuiFonction = [&aboutWindow]{ aboutWindow.gui(); };
    };

    //
    // Set our app dockable windows list
    //
    runnerParams.dockingParams.dockableWindows = {
        dock_imguiDemoCode,
        dock_imguiDemoWindow,
        dock_imguiReadme,
        dock_imGuiCppDocBrowser,
        dock_imguiCodeBrowser,
        dock_acknowledgments,
        dock_about
    };

    // Set the app menu
    runnerParams.callbacks.ShowMenus = menuTheme;
    // Set the custom fonts
    runnerParams.callbacks.LoadAdditionalFonts = MarkdownHelper::LoadFonts;

    // Ready, set, go!
    HelloImGui::Run(runnerParams);
    return 0;
}
