#include "hello_imgui/hello_imgui.h"
int main(int , char *[])
{
    HelloImGui::RunnerParams runnerParams;      // runnerParams will contains all the application params and callbacks
    runnerParams.callbacks.ShowGui =            // ShowGui contains a lambda function with the Gui code
        [&runnerParams]{
            ImGui::ShowDemoWindow();
        };
    runnerParams.appWindowParams.windowTitle = "imgui_demo_hellorun";
    runnerParams.appWindowParams.windowSize = {1280.f, 720.f};
    HelloImGui::Run(runnerParams);
    return 0;
}
