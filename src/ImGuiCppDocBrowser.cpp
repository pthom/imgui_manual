#include "source_parse/ImGuiRepoUrl.h"

#include "ImGuiCppDocBrowser.h"
#include "hello_imgui/hello_imgui.h"
#include "imgui_utilities/HyperlinkHelper.h"
#include "imgui_utilities/ImGuiExt.h"
#include "source_parse/ImGuiCodeParser.h"

ImGuiCppDocBrowser::ImGuiCppDocBrowser()
    : WindowWithEditor()
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
            "imgui.cpp contains some useful Docs\n";
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
                          + std::to_string(mEditor.GetCursorPosition().mLine);
        HyperlinkHelper::OpenUrl(url);
    }
}
