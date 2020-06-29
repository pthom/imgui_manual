#include "hello_imgui.h"

#include "imgui_markdown.h"       // https://github.com/juliettef/imgui_markdown
#include "MarkdownHelper.h"
#include <fplus/fplus.hpp>

#if defined(__EMSCRIPTEN__)
    #include <emscripten.h>
#elif defined(_WIN32)
    #include <windows.h>
    #include <Shellapi.h>
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #include <cstdlib>
#endif

namespace MarkdownHelper
{

ImFont *fontH1, *fontH2, *fontH3;

void LoadFonts()
{
    HelloImGui::ImGuiDefaultSettings::LoadDefaultFont_WithFontAwesomeIcons();
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
    {
        bool isAbsoluteUrl = fplus::is_prefix_of(std::string("http"), url);
        if (!isAbsoluteUrl)
            return;
        #if defined(__EMSCRIPTEN__)
            std::string js_command = "window.open(\"" + url + "\");";
            emscripten_run_script(js_command.c_str());
        #elif defined(_WIN32)
            ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
        #elif defined(TARGET_OS_MAC)
            std::string cmd = std::string("open ") + url.c_str();
            system(cmd.c_str());
        #endif
    }
}

inline ImGui::MarkdownImageData ImageCallback(ImGui::MarkdownLinkCallbackData data_)
{
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
    static ImGui::MarkdownConfig markdownConfig = factorMarkdownConfig();
    ImGui::Markdown(markdown_.c_str(), markdown_.length(), markdownConfig);
}


} // namespace MarkdownHelper
