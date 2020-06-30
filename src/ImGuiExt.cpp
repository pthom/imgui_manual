#include "ImGuiExt.h"
#include "imgui.h"
#include "hello_imgui.h"
#include "HyperlinkHelper.h"

namespace ImGuiExt
{
    bool ClickableText(const std::string &txt)
    {
        auto blue = ImVec4(0.35f, 0.5f, 0.85f, 1.f);
        ImGui::TextColored(blue, "%s", txt.c_str());
        if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        return (ImGui::IsItemHovered() && ImGui::IsMouseDown(0));
    }

    void Hyperlink(const std::string &url)
    {
        std::string linkLabel = std::string(ICON_FA_LINK) + " ##" + url;
        if (ImGui::Button(linkLabel.c_str()))
            HyperlinkHelper::OpenUrl(url);
        ImGui::SameLine();
        auto blue = ImVec4(0.3f, 0.5f, 1.f, 1.f);
        ImGui::TextColored(blue, "%s", url.c_str());
        if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0))
            HyperlinkHelper::OpenUrl(url);
    }

    void SameLine_IfPossible(float minRightMargin)
    {
        auto lastXPos = ImGui::GetItemRectMax().x - ImGui::GetWindowPos().x;
        auto windowWidth = ImGui::GetWindowSize().x;
        if (lastXPos < windowWidth - minRightMargin )
            ImGui::SameLine();
    }
}