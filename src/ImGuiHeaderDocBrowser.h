#pragma once
#include "source_parse/GuiHeaderTree.h"
#include "source_parse/Sources.h"
#include "WindowWithEditor.h"

// This windows shows the docs contained in imgui.cpp
class ImGuiHeaderDocBrowser: public WindowWithEditor
{
public:
    ImGuiHeaderDocBrowser();
    void gui();
    void searchForTermInImGuiHeader(const std::string& search);
private:
    void guiTags();
    void guiGithubButton();

    SourceParse::AnnotatedSource mAnnotatedSource;
    SourceParse::GuiHeaderTree mGuiHeaderTree;
};

void searchForTermInImGuiHeader(const std::string& search);
