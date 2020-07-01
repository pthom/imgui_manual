#include <fplus/fplus.hpp>
#include "LibrariesCodeBrowser.h"
#include "utilities/MarkdownHelper.h"
#include "utilities/ImGuiExt.h"

LibrariesCodeBrowser::LibrariesCodeBrowser(
    const std::vector<Sources::Library> &librarySources,
    std::string currentSourcePath)
        : WindowWithEditor()
        , mLibraries(librarySources)
        , mCurrentSource(Sources::ReadSource(currentSourcePath))
{
    mEditor.SetText(mCurrentSource.sourceCode);
}

void LibrariesCodeBrowser::gui()
{
    if (guiSelectLibrarySource())
        mEditor.SetText(mCurrentSource.sourceCode);

    if (fplus::is_suffix_of(std::string(".md"), mCurrentSource.sourcePath))
        MarkdownHelper::Markdown(mCurrentSource.sourceCode);
    else
        RenderEditor(mCurrentSource.sourcePath.c_str());
}

bool LibrariesCodeBrowser::guiSelectLibrarySource()
{
    bool changed = false;
    for (const auto & librarySource: mLibraries)
    {
        ImGui::Text("%s", librarySource.name.c_str());
        ImGui::SameLine(ImGui::GetWindowSize().x - 350.f );
        ImGuiExt::Hyperlink(librarySource.url);
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
                mCurrentSource = Sources::ReadSource(currentSourcePath);
                changed = true;
            }
            ImGuiExt::SameLine_IfPossible(80.f);
        }
        ImGui::NewLine();
        ImGui::Separator();
    }
    return changed;
}
