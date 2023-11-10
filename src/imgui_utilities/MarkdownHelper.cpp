#include "hello_imgui/hello_imgui.h"

#include "imgui_markdown.h"       // https://github.com/juliettef/imgui_markdown
#include "MarkdownHelper.h"
#include "HyperlinkHelper.h"
#include <fplus/fplus.hpp>

namespace MarkdownHelper
{

ImFont *fontH1, *fontH2, *fontH3;

void LoadFonts()
{
    std::string fontFilename = "fonts/DroidSans.ttf";
    float fontSizeStep = 4.;
    fontH3 = HelloImGui::LoadFontTTF_WithFontAwesomeIcons(fontFilename, 14.f + fontSizeStep * 1.f);
    fontH2 = HelloImGui::LoadFontTTF_WithFontAwesomeIcons(fontFilename, 14.f + fontSizeStep * 2.f);
    fontH1 = HelloImGui::LoadFontTTF_WithFontAwesomeIcons(fontFilename, 14.f + fontSizeStep * 3.f);
}


void LinkCallback(ImGui::MarkdownLinkCallbackData data_)
{
    (void) data_;
    std::string url(data_.link, data_.linkLength);
    if (!data_.isImage)
        HyperlinkHelper::OpenUrl(url);
}

inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_)
{
    (void)data_;
    // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
    ImTextureID image = ImGui::GetIO().Fonts->TexID;
    ImGui::MarkdownImageData imageData{true, false, image, ImVec2(40.0f, 20.0f)};
    return imageData;
}

// You can make your own Markdown function with your prefered string container and markdown config.
ImGui::MarkdownConfig factorMarkdownConfig()
{
    return {
            LinkCallback,
            ImageCallback,
            ICON_FA_LINK,
            {
                    {fontH1, true},
                    {fontH2, true},
                    {fontH3, false}
            }
    };
}


void Markdown(const std::string &markdown_)
{
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 1.f, 1.f)); // Hack MarkDown links color, which use ImGuiCol_ButtonHovered
    static ImGui::MarkdownConfig markdownConfig = factorMarkdownConfig();
    ImGui::Markdown(markdown_.c_str(), markdown_.length(), markdownConfig);
    ImGui::PopStyleColor();
}


} // namespace MarkdownHelper
