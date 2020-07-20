#include "imgui.h"
#include "hello_imgui/hello_imgui.h"

void Playground();

int main(int, char **)
{
    HelloImGui::RunnerParams params;
    params.imGuiWindowParams.defaultImGuiWindowType =
        HelloImGui::DefaultImGuiWindowType::NoDefaultWindow;
    params.callbacks.ShowGui = Playground;
    HelloImGui::Run(params);
}
