#include "playground.h"

void Playground()
{
    static int counter = 0;
    if (ImGui::Button("Click me"))
        ++counter;
    ImGui::Text("counter=%i", counter);
}
