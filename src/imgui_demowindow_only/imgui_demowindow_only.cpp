#include "hello_imgui/hello_imgui.h"

int main(int , char *[])
{
    HelloImGui::RunnerParams params;
    params.appWindowParams.windowSize = {1280.f, 720.f};
    params.appWindowParams.windowTitle = "ImGui Demo Window Only";
    params.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::NoDefaultWindow;

    params.callbacks.ShowGui = [&]() { ImGui::ShowDemoWindow(); };
    HelloImGui::Run(params);
    return 0;
}
