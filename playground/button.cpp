#include "hello_imgui/hello_imgui.h"

void Gui()
{
    static int counter = 0;
    if (ImGui::Button("Click me"))
        ++counter;
    ImGui::Text("counter=%i", counter);
}

int main(int, char **)
{
  HelloImGui::Run(Gui);
  return 0;
}
