#pragma once
#include "WindowWithEditor.h"
#include "LibrariesCodeBrowser.h"

class AboutWindow
{
public:
    AboutWindow();
    void gui();
private:
    void guiHelp();

    LibrariesCodeBrowser mLibrariesCodeBrowser;
};

