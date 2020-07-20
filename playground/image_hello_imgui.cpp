#include "playground.h"

void Playground()
{
    static auto image = HelloImGui::ImageGl::FactorImage("smiley.png");
    image->Draw(ImVec2(300.f, 300.f));
}
