#pragma once
#include <string>

namespace ImGuiExt
{
    void Hyperlink(const std::string &url);
    void SameLine_IfPossible(float minRightMargin = 60.f);
    bool ClickableText(const std::string &txt);

    bool Button_WithEnabledFlag(const char *label, bool enabled, const char *tooltip = nullptr, bool sameLineAfter = false);
    bool SmallButton_WithEnabledFlag(const char *label, bool enabled, const char *tooltip = nullptr, bool sameLineAfter = false);
}
