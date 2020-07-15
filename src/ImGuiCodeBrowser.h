#pragma once
#include "LibrariesCodeBrowser.h"
#include "sources/Sources.h"
#include "WindowWithEditor.h"

// This window shows ImGui source files + Readme/License
class ImGuiCodeBrowser
{
public:
    ImGuiCodeBrowser();
    void gui();
private:
    inline void guiHelp();

    LibrariesCodeBrowser mLibrariesCodeBrowser;
};
