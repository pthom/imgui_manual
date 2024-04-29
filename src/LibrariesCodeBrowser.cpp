#include "LibrariesCodeBrowser.h"
#include "imgui_utilities/MarkdownHelper.h"
#include "imgui_utilities/ImGuiExt.h"
#include "hello_imgui/hello_imgui.h"
#include <fplus/fplus.hpp>

namespace
{
    std::string MarkdownLinkString(const std::string &url)
    {
        return std::string("[") + url + "]" + "(" + url + ")";
    }
}


LibrariesCodeBrowser::LibrariesCodeBrowser(
    const std::string & windowName,
    const std::vector<SourceParse::Library> &librarySources,
    std::string currentSourcePath)
        : WindowWithEditor(windowName)
        , mLibraries(librarySources)
{
    if (!currentSourcePath.empty())
        mCurrentSource = SourceParse::ReadSource(currentSourcePath);
    mEditor.SetText(mCurrentSource.sourceCode);
}

void LibrariesCodeBrowser::gui()
{
    if (guiSelectLibrarySource())
        mEditor.SetText(mCurrentSource.sourceCode);

    std::string sourcePath = mCurrentSource.sourcePath;
    if (fplus::is_suffix_of(std::string(".md"), sourcePath))
        MarkdownHelper::Markdown(mCurrentSource.sourceCode);
    else if (fplus::is_suffix_of(std::string(".png"), sourcePath))
    {
        HelloImGui::ImageFromAsset(sourcePath.c_str(), ImVec2(ImGui::GetWindowSize().x - 30.f, 0.f));
    }
    else
        RenderEditor(mCurrentSource.sourcePath.c_str());
}

bool LibrariesCodeBrowser::guiSelectLibrarySource()
{
    bool changed = false;
    for (const auto & librarySource: mLibraries)
    {
        ImGui::Text("%s", librarySource.name.c_str());
        ImGui::SameLine(150.f);
        MarkdownHelper::Markdown(MarkdownLinkString(librarySource.url));
        MarkdownHelper::Markdown(librarySource.shortDoc);
        for (const auto & source: librarySource.sourcePaths)
        {
            std::string currentSourcePath = librarySource.path + "/" + source;
            bool isSelected = (currentSourcePath == mCurrentSource.sourcePath);
            std::string buttonLabel = source + "##" + librarySource.path;
            if (isSelected)
                ImGui::TextDisabled("%s", source.c_str());
            else if (ImGui::Button(buttonLabel.c_str()))
            {
                mCurrentSource = SourceParse::ReadSource(currentSourcePath);
                changed = true;
            }
            ImGuiExt::SameLine_IfPossible(150.f);
        }
        ImGui::NewLine();
        ImGui::Separator();
    }
    return changed;
}
