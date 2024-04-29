#include "source_parse/ImGuiRepoUrl.h"

#include "ImGuiCppDocBrowser.h"
#include "hello_imgui/hello_imgui.h"
#include "imgui_utilities/HyperlinkHelper.h"
#include "imgui_utilities/ImGuiExt.h"
#include "source_parse/ImGuiCodeParser.h"
#include "hello_imgui/icons_font_awesome_4.h"

ImGuiCppDocBrowser::ImGuiCppDocBrowser()
    : WindowWithEditor("imgui.cpp - Doc")
    , mAnnotatedSource(SourceParse::ReadImGuiCppDoc())
    , mGuiHeaderTree(mAnnotatedSource.linesWithTags)
{
    setEditorAnnotatedSource(mAnnotatedSource);
}

void ImGuiCppDocBrowser::gui()
{
    static bool showHelp = true;
    if (showHelp)
    {
        std::string help =
            "The high level doc for Dear ImGui is simply stored inside imgui.cpp\n";
        ImGui::TextWrapped("%s", help.c_str());
        if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
        showHelp = false;
    }
    guiTags();
    RenderEditor("imgui.cpp", [this] { this->guiGithubButton(); });
}

void ImGuiCppDocBrowser::guiTags()
{
    int currentEditorLineNumber = mEditor.GetCursorPosition().mLine;
    int selectedLine = mGuiHeaderTree.gui(currentEditorLineNumber);
    if (selectedLine >= 0)
        mEditor.SetCursorPosition({selectedLine, 0}, 3);
}

void ImGuiCppDocBrowser::guiGithubButton()
{
    if (ImGui::SmallButton("View on github at this line"))
    {
        std::string url = ImGuiRepoUrl() + "imgui.cpp#L"
                          + std::to_string(mEditor.GetCursorPosition().mLine + 1);
        HyperlinkHelper::OpenUrl(url);
    }
}
