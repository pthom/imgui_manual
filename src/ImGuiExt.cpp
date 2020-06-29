#include "ImGuiExt.h"
#include "imgui.h"
#include "hello_imgui.h"
#include "HyperlinkHelper.h"

namespace ImGuiExt
{
    void Hyperlink(const std::string &url)
    {
        std::string linkLabel = std::string(ICON_FA_LINK) + " ##" + url;
        if (ImGui::Button(linkLabel.c_str()))
            HyperlinkHelper::OpenUrl(url);
        ImGui::SameLine();
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