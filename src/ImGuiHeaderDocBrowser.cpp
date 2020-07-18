#include "source_parse/ImGuiRepoUrl.h"

#include "hello_imgui/hello_imgui.h"
#include "imgui_utilities/HyperlinkHelper.h"
#include "imgui_utilities/ImGuiExt.h"
#include "source_parse/ImGuiCodeParser.h"
#include "source_parse/Sources.h"

#include "ImGuiHeaderDocBrowser.h"

ImGuiHeaderDocBrowser::ImGuiHeaderDocBrowser()
    : WindowWithEditor()
    , mAnnotatedSource(SourceParse::ReadImGuiHeaderDoc())
    , mGuiHeaderTree(mAnnotatedSource.linesWithTags)
{
    setEditorAnnotatedSource(mAnnotatedSource);
}

void ImGuiHeaderDocBrowser::gui()
{
    static bool showHelp = true;
    if (showHelp)
    {
        std::string help =
            "imgui.h contains many useful advices. See the searchable table of content below (and for example, "
            "search for \"docking\")";
        ImGui::TextWrapped("%s", help.c_str());
        if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
            showHelp = false;
    }
    guiTags();
    RenderEditor("imgui.h", [this] { this->guiGithubButton(); });
}

void ImGuiHeaderDocBrowser::guiTags()
{
    int currentEditorLineNumber = mEditor.GetCursorPosition().mLine;
    int selectedLine = mGuiHeaderTree.gui(currentEditorLineNumber);
    if (selectedLine >= 0)
        mEditor.SetCursorPosition({selectedLine, 0}, 3);
}

void ImGuiHeaderDocBrowser::guiGithubButton()
{
    if (ImGui::SmallButton("View on github at this line"))
    {
        std::string url = ImGuiRepoUrl() + "imgui.h#L"
                          + std::to_string(mEditor.GetCursorPosition().mLine + 1);
        HyperlinkHelper::OpenUrl(url);
    }
}
