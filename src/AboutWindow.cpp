#include "hello_imgui/hello_imgui.h"
#include "utilities/MarkdownHelper.h"
#include "Sources.h"
#include "AboutWindow.h"

AboutWindow::AboutWindow()
    : mLibrariesCodeBrowser(Sources::imguiManualLibrary(), "imgui_manual/ImGuiManual.cpp")
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
This interactive manual was developed using [Hello ImGui](https://github.com/pthom/hello_imgui), which provided the emscripten port, as well as the assets and source code embedding.
See also a [related demo for Implot](https://traineq.org/implot_demo/src/implot_demo.html), which also provides code navigation.
)";
        MarkdownHelper::Markdown(help.c_str());
        if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
            showHelp = false;
    }
}