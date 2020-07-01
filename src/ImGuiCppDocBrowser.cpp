#include "ImGuiRepoUrl.h"

#include "utilities/ImGuiExt.h"
#include "utilities/HyperlinkHelper.h"

#include <fplus/fplus.hpp>

#include "ImGuiCppDocBrowser.h"

ImGuiCppDocBrowser::ImGuiCppDocBrowser()
    : WindowWithEditor()
    , mAnnotatedSource(Sources::ReadImGuiCppDoc("imgui/imgui.cpp"))
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
        // tags are of type H1 or H2, and begin with "H1 " or "H2 " (3 characters)
        std::string title = fplus::drop(3, lineWithTag.tag);
        bool isHeader1 = (fplus::take(3, lineWithTag.tag) == "H1 ");
        if (isHeader1)
        {
            if (ImGuiExt::ClickableText(title.c_str()))
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
