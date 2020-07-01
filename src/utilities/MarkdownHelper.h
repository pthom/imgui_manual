#pragma once

namespace MarkdownHelper
{
    extern ImFont *fontH1, *fontH2, *fontH3;

    void LoadFonts();
    void Markdown(const std::string &markdown_);
}