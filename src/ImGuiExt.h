#pragma once
#include <string>

namespace ImGuiExt
{
    void Hyperlink(const std::string &url);
    void SameLine_IfPossible(float minRightMargin = 60.f);
}
