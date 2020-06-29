#include "hello_imgui/hello_imgui.h"
#include "implot.h"

int main()
{
    HelloImGui::RunnerParams runnerParams;

    runnerParams.imGuiWindowParams.defaultImGuiWindowType =
            HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
    runnerParams.appWindowParams.windowTitle = "implot demo";
    runnerParams.appWindowParams.windowSize = { 1200, 800};
    runnerParams.imGuiWindowParams.showMenuBar = true;

    auto showImPlot = [] {
      ImPlot::ShowDemoWindow(nullptr);
    };
    auto showCode = [] {
      ImGui::Text("Code goes here");
    };

    runnerParams.dockingParams.dockingSplits =
    {
        { "MainDockSpace", "RightSpace", ImGuiDir_Right, 0.5 },
    };
    runnerParams.dockingParams.dockableWindows =
    {
        { "ImPlot Demo", "MainDockSpace", showImPlot },
        { "Code", "RightSpace", showCode },
    };

    HelloImGui::Run(runnerParams);
}