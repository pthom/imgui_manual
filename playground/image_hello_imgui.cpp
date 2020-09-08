#include "playground.h"

void Playground()
{
#ifdef HELLOIMGUI_HAS_OPENGL
    static auto image = HelloImGui::ImageGl::FactorImage("smiley.png");
    image->Draw(ImVec2(300.f, 300.f));
#endif // #ifdef HELLOIMGUI_HAS_OPENGL
}
