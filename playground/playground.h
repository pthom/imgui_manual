#include "imgui.h"
#include "hello_imgui/hello_imgui.h"

#define DemoCode(x)
#define DemoCode_(x)

void Playground();

static void ShowDockingDisabledMessage()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
    ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
    ImGui::SameLine(0.0f, 0.0f);
    if (ImGui::SmallButton("click here"))
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}


int main(int, char **)
{
    HelloImGui::Run(Playground);
}
