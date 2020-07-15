#include "imgui_utilities/MarkdownHelper.h"

#include "hello_imgui/hello_imgui.h"

#include "Acknowledgments.h"

Acknowledgments::Acknowledgments()
    : mLibrariesCodeBrowser(SourceParse::acknowldegmentLibraries(), "")
{
}

void Acknowledgments::gui()
{
    guiHelp();
    mLibrariesCodeBrowser.gui();
}

void Acknowledgments::guiHelp()
{
    static bool showHelp = true;
    if (showHelp)
    {
        std::string help = "This manual uses some great libraries, which are shown below.";
        MarkdownHelper::Markdown(help.c_str());
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
            showHelp = false;
    }
}
