#include "hello_imgui/hello_imgui.h"
#include "implot.h"

int main()
{
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
    runnerParams.dockingParams.dockingSplits =
    {
            { "MainDockSpace", "RightSpace", ImGuiDir_Right, 0.5 },
    };

    // Dockable windows
    HelloImGui::DockableWindow implotDock;
    {
        implotDock.label = "ImPlot Demo";
        implotDock.dockSpaceName = "MainDockSpace";
        implotDock.GuiFonction = [&implotDock] {
          ImPlot::ShowDemoWindow(&implotDock.isVisible);
        };
    };

    HelloImGui::DockableWindow codeDock;
    {
        codeDock.label = "Code";
        codeDock.dockSpaceName = "RightSpace";
        codeDock.GuiFonction = [] {
          ImGui::Text("Code goes here");
        };
    }

    runnerParams.dockingParams.dockableWindows = { implotDock, codeDock };

    HelloImGui::Run(runnerParams);
}