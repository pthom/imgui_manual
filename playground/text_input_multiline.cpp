#include "hello_imgui/hello_imgui.h"

void Gui()
{
    // Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
    // and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
    static char text[1024 * 16] =
        "/*\n"
        " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
        " the hexadecimal encoding of one offending instruction,\n"
        " more formally, the invalid operand with locked CMPXCHG8B\n"
        " instruction bug, is a design flaw in the majority of\n"
        " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
        " processors (all in the P5 microarchitecture).\n"
        "*/\n\n"
        "label:\n"
        "\tlock cmpxchg8b eax\n";
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", (unsigned int*)&flags, ImGuiInputTextFlags_ReadOnly);
    ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", (unsigned int*)&flags, ImGuiInputTextFlags_AllowTabInput);
    ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine", (unsigned int*)&flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
    ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);

}

int main(int, char **)
{
  HelloImGui::Run(Gui);
  return 0;
}
