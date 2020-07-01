#pragma once
#include "Sources.h"
#include "WindowWithEditor.h"

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
    Sources::Source mCurrentSource;
};
