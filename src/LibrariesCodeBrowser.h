#pragma once
#include "source_parse/Sources.h"
#include "WindowWithEditor.h"
#include "hello_imgui/hello_imgui.h"
#include <unordered_map>


class LibrariesCodeBrowser: public WindowWithEditor
{
public:
    LibrariesCodeBrowser(
        const std::string & windowName,
        const std::vector<SourceParse::Library>& librarySources,
        std::string currentSourcePath
    );
    void gui();
private:
    bool guiSelectLibrarySource();

    std::vector<SourceParse::Library> mLibraries;
    std::unordered_map<std::string, HelloImGui::ImageGlPtr> mTextureCache;
    SourceParse::Source mCurrentSource;
};
