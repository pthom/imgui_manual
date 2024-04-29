#include "hello_imgui/hello_imgui.h"
#include "imgui_utilities/MarkdownHelper.h"
#include "hello_imgui/icons_font_awesome_4.h"
#include "source_parse/Sources.h"
#include "AboutWindow.h"

AboutWindow::AboutWindow()
    : mLibrariesCodeBrowser("AboutWindow", SourceParse::imguiManualLibrary(), "imgui_manual/ImGuiManual.cpp")
{
}

void AboutWindow::gui()
{
    guiHelp();
    mLibrariesCodeBrowser.gui();
}

void AboutWindow::guiHelp()
{
    static bool showHelp = true;
    if (showHelp)
    {
        std::string help = R"(
Note: an online playground provided with this manual enables you to test ImGui without any installation:
* [see a demo](https://youtu.be/FJgObNNmuzo)
* [launch the playground](https://gitpod.io/#https://github.com/pthom/imgui_manual).

This interactive manual was developed using [Hello ImGui](https://github.com/pthom/hello_imgui), which provided the emscripten port, as well as the assets embedding and image loading. ImGuiManual.cpp gives a good overview of [Hello Imgui API](https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/hello_imgui_api.md).

See also a [related demo for Implot](https://traineq.org/implot_demo/src/implot_demo.html).

[I'd love to read your feedback!](https://github.com/pthom/imgui_manual/issues/1)
)";
        MarkdownHelper::Markdown(help.c_str());
        if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
            showHelp = false;
    }
}
