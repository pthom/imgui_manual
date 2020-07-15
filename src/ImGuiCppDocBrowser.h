#pragma once
#include "sources/Sources.h"
#include "WindowWithEditor.h"

// This windows shows the docs contained in imgui.cpp
class ImGuiCppDocBrowser: public WindowWithEditor
{
public:
    ImGuiCppDocBrowser();
    void gui();

private:
    void guiTags();
    void guiGithubButton();

    Sources::AnnotatedSource mAnnotatedSource;
};
