#include "ImGuiExt.h"
#include "imgui.h"
#include "hello_imgui.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace ImGuiExt
{
    void Hyperlink(const std::string &url)
    {
#ifdef __EMSCRIPTEN__
        std::string linkLabel = std::string(ICON_FA_LINK) + " ##" + url;
    if (ImGui::Button(linkLabel.c_str()))
    {
        std::string js_command = "window.open(\"" + url + "\");";
        emscripten_run_script(js_command.c_str());
    }
    ImGui::SameLine();
#endif
        auto blue = ImVec4(0.3f, 0.5f, 1.f, 1.f);
        ImGui::TextColored(blue, "%s", url.c_str());
    }

    void SameLine_IfPossible(float minRightMargin)
    {
        auto lastXPos = ImGui::GetItemRectMax().x;
        //ImGui::Pos
        auto windowWidth = ImGui::GetWindowSize().x;
        if (lastXPos < windowWidth - minRightMargin )
            ImGui::SameLine();
    }
}