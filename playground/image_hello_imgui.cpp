#include "hello_imgui/hello_imgui.h"

void Gui()
{
    static auto image = HelloImGui::ImageGl::FactorImage("smiley.png");
    image->Draw(ImVec2(300.f, 300.f));
}

int main(int, char **)
{
  HelloImGui::Run(Gui);
  return 0;
}
