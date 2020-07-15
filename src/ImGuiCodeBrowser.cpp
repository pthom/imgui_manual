#include "imgui_utilities/MarkdownHelper.h"
#include "hello_imgui/hello_imgui.h"

#include "ImGuiCodeBrowser.h"


ImGuiCodeBrowser::ImGuiCodeBrowser()
    : mLibrariesCodeBrowser(SourceParse::imguiLibrary(), "imgui/imgui.h")
{
}

void ImGuiCodeBrowser::gui()
{
    guiHelp();
    mLibrariesCodeBrowser.gui();
}

void ImGuiCodeBrowser::guiHelp()
{
    static bool showHelp = true;
    if (showHelp)
    {
        // Readme
        std::string help = R"(
This is the core of ImGui code.

Usage (extract from [ImGui Readme](https://github.com/ocornut/imgui#usage))

The core of Dear ImGui is self-contained within a few platform-agnostic files which you can easily compile in your application/engine. They are all the files in the root folder of the repository (imgui.cpp, imgui.h, imgui_demo.cpp, imgui_draw.cpp etc.).

No specific build process is required. You can add the .cpp files to your existing project.

You will need a backend to integrate Dear ImGui in your app. The backend passes mouse/keyboard/gamepad inputs and variety of settings to Dear ImGui, and is in charge of rendering the resulting vertices.

Backends for a variety of graphics api and rendering platforms are provided in the [examples/](https://github.com/ocornut/imgui/tree/master/examples) folder, along with example applications. See the [Integration](https://github.com/ocornut/imgui#integration) section of this document for details. You may also create your own backend. Anywhere where you can render textured triangles, you can render Dear ImGui.
)";
        MarkdownHelper::Markdown(help.c_str());
        if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
        showHelp = false;
    }
}
