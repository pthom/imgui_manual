#include "source_parse/ImGuiRepoUrl.h"

#include "imgui_utilities/ImGuiExt.h"
#include "imgui_utilities/HyperlinkHelper.h"

#include "ImGuiCppDocBrowser.h"

ImGuiCppDocBrowser::ImGuiCppDocBrowser()
    : WindowWithEditor()
    , mAnnotatedSource(SourceParse::ReadImGuiCppDoc())
{
    setEditorAnnotatedSource(mAnnotatedSource);
}

void ImGuiCppDocBrowser::gui()
{
    ImGui::Text("The doc for Dear ImGui is simply stored inside imgui.cpp");
    guiTags();
    RenderEditor("imgui.cpp", [this] { this->guiGithubButton(); });
}

void ImGuiCppDocBrowser::guiTags()
{
    for (auto lineWithTag : mAnnotatedSource.linesWithTags)
    {
        if (lineWithTag.level == 1)
        {
            if (ImGuiExt::ClickableText(lineWithTag.tag.c_str()))
                mEditor.SetCursorPosition({lineWithTag.lineNumber, 0}, 3);
        }
    }
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
