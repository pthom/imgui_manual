#include "ImGuiExt.h"
#include "imgui.h"
#include "hello_imgui.h"
#include "HyperlinkHelper.h"
#include "MarkdownHelper.h"
#include <cstring>

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
    
    void SameLine_IfPossible(float minRightMargin)
    {
        auto lastXPos = ImGui::GetItemRectMax().x - ImGui::GetWindowPos().x;
        auto windowWidth = ImGui::GetWindowSize().x;
        if (lastXPos < windowWidth - minRightMargin )
            ImGui::SameLine();
    }

    bool Button_WithEnabledFlag(const char *label, bool enabled, const char *tooltip, bool sameLineAfter)
    {
        if (!enabled)
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
        }
        bool clicked = ImGui::Button(label);
        if ((strlen(tooltip) > 0) && ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", tooltip);
        if (!enabled)
            ImGui::PopStyleColor(3);
        if (sameLineAfter)
            ImGui::SameLine();
        return enabled ? clicked : false;
    }

    bool SmallButton_WithEnabledFlag(const char *label, bool enabled, const char *tooltip, bool sameLineAfter)
    {
        if (!enabled)
        {
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyle().Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyle().Colors[ImGuiCol_Button]);
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
        }
        bool clicked = ImGui::SmallButton(label);
        if ((strlen(tooltip) > 0) && ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", tooltip);
        if (!enabled)
            ImGui::PopStyleColor(3);
        if (sameLineAfter)
            ImGui::SameLine();
        return enabled ? clicked : false;
    }
}