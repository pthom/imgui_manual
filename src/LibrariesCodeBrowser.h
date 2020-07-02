#pragma once
#include "Sources.h"
#include "WindowWithEditor.h"
#include "hello_imgui/hello_imgui.h"
#include <unordered_map>


class LibrariesCodeBrowser: public WindowWithEditor
{
public:
    LibrariesCodeBrowser(
        const std::vector<Sources::Library>& librarySources,
        std::string currentSourcePath
    );
    void gui();
private:
    bool guiSelectLibrarySource();

    std::vector<Sources::Library> mLibraries;
    std::unordered_map<std::string, HelloImGui::ImageGlPtr> mTextureCache;
    Sources::Source mCurrentSource;
};
