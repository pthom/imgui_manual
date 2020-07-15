#pragma once
#include "sources/Sources.h"
#include "utilities/MarkdownHelper.h"

// This windows shows ImGui's Readme.md
class ImGuiReadmeBrowser
{
public:
    ImGuiReadmeBrowser() : mSource(Sources::ReadSource("imgui/README.md")) {}
    inline void gui()
    {
        MarkdownHelper::Markdown(mSource.sourceCode);
    }
private:
    Sources::Source mSource;
};
