#pragma once
#include <memory>

#include "WindowWithEditor.h"
#include "source_parse/Sources.h"
#include "source_parse/GuiHeaderTree.h"

enum class ViewPythonOrCpp { Cpp, Python };

// This is the window that shows imgui_demo.cpp code,
// with a callback that will point to the correct line number
// (see implImGuiDemoCallbackDemoCallback)
class ImGuiDemoBrowser
{
public:
    ImGuiDemoBrowser();
    void gui();
    void ImGuiDemoCallback(const char* file, int line_number, const char* demo_title);

private:
    void guiHelp();
    void guiSave();
    void guiEditorAdditional();
    void guiDemoCodeTags();

    struct SourceElements
    {
        SourceParse::AnnotatedSource AnnotatedSource;
        SourceParse::GuiHeaderTree_FollowDemo mGuiHeaderTree;
        WindowWithEditor mWindowWithEditor;

        SourceElements(const SourceParse::AnnotatedSource& as, const std::string& editorLabel)
            : AnnotatedSource(as), mGuiHeaderTree(as.linesWithTags), mWindowWithEditor(editorLabel)
        {
            mWindowWithEditor.InnerTextEditor().SetText(as.source.sourceCode);
        }
    };

    SourceElements& CurrentSourceElements()
    {
        return mViewPythonOrCpp == ViewPythonOrCpp::Cpp ? mSourceElementsCpp : mSourceElementsPython;
    }

    std::vector<SourceElements *> AllSourceElements(){ return {&mSourceElementsCpp, &mSourceElementsPython}; }

    SourceElements mSourceElementsCpp;
    SourceElements mSourceElementsPython;

    ViewPythonOrCpp mViewPythonOrCpp = ViewPythonOrCpp::Cpp;

};
