#include "ImGuiExt.h"
#include "imgui.h"
#include "hello_imgui.h"
#include "HyperlinkHelper.h"

namespace ImGuiExt
{
    bool ClickableText(const std::string &txt)
    {
        auto blue = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImGui::TextColored(blue, "%s", txt.c_str());
        if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        return (ImGui::IsItemHovered() && ImGui::IsMouseDown(0));
    }

    void Hyperlink(const std::string &url)
    {
        auto blue = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImGui::TextColored(blue, "%s", url.c_str());
        if (ImGui::IsItemHovered() || ImGui::IsItemFocused())
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
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