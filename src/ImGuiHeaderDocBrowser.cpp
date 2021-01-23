#include "source_parse/ImGuiRepoUrl.h"

#include "hello_imgui/hello_imgui.h"
#include "imgui_utilities/HyperlinkHelper.h"
#include "imgui_utilities/ImGuiExt.h"
#include "source_parse/ImGuiCodeParser.h"
#include "source_parse/Sources.h"

#include "ImGuiHeaderDocBrowser.h"

ImGuiHeaderDocBrowser * gInstance;

ImGuiHeaderDocBrowser::ImGuiHeaderDocBrowser()
    : WindowWithEditor("imgui.h - Doc")
    , mAnnotatedSource(SourceParse::ReadImGuiHeaderDoc())
    , mGuiHeaderTree(mAnnotatedSource.linesWithTags)
{
    setEditorAnnotatedSource(mAnnotatedSource);
    gInstance = this;
}

void ImGuiHeaderDocBrowser::gui()
{
    std::string help =
        "imgui.h contains the detailed doc for the different widgets and features.\n"
        "See the searchable table of content below (and for example, search for \"docking\")";
    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.f, 1.0f), "(?)");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("%s", help.c_str());
    ImGui::SameLine(50.f);
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
