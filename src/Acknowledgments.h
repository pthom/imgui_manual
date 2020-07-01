#pragma once
#include "LibrariesCodeBrowser.h"
#include "WindowWithEditor.h"

// This window acknowledges the different libraries used by this manual
class Acknowledgments
{
public:
    Acknowledgments();
    void gui();
private:
    void guiHelp();

    LibrariesCodeBrowser mLibrariesCodeBrowser;
};
