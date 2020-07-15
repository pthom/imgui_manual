#pragma once
#include "source_parse/Sources.h"
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

    SourceParse::AnnotatedSource mAnnotatedSource;
};
