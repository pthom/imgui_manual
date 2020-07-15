#include "sources/ImGuiRepoUrl.h"

#include "utilities/ImGuiExt.h"
#include "utilities/HyperlinkHelper.h"
#include "sources/ImGuiHeaderDocParser.h"
#include "sources/Sources.h"
#include <fplus/fplus.hpp>

#include "ImGuiHeaderDocBrowser.h"

ImGuiHeaderDocBrowser::ImGuiHeaderDocBrowser()
    : WindowWithEditor()
    , mAnnotatedSource(Sources::ReadImGuiHeaderDoc())
{
    setEditorAnnotatedSource(mAnnotatedSource);
}

void ImGuiHeaderDocBrowser::gui()
{
    ImGui::Text("The doc for Dear ImGui is simply stored inside imgui.cpp");
    guiTags();
    RenderEditor("imgui.cpp", [this] { this->guiGithubButton(); });
}

void ImGuiHeaderDocBrowser::guiTags()
{
//    for (auto lineWithTag : mAnnotatedSource.linesWithTags)
//    {
//        if (lineWithTag.level == 1)
//        {
//            if (ImGuiExt::ClickableText(lineWithTag.tag.c_str()))
//                mEditor.SetCursorPosition({lineWithTag.lineNumber, 0}, 3);
//        }
//    }
    int currentEditorLineNumber = mEditor.GetCursorPosition().mLine;
    int selectedLine = Sources::guiLinesWithTags(mAnnotatedSource.linesWithTags, currentEditorLineNumber);
    if (selectedLine >= 0)
        mEditor.SetCursorPosition({selectedLine, 0}, 3);
}

void ImGuiHeaderDocBrowser::guiGithubButton()
{
    if (ImGui::SmallButton("View on github at this line"))
    {
        std::string url = ImGuiRepoUrl() + "imgui.h#L"
                          + std::to_string(mEditor.GetCursorPosition().mLine);
        HyperlinkHelper::OpenUrl(url);
    }
}
