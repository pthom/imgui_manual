#pragma once
#include "source_parse/Sources.h"
#include "TextEditor.h"
#include "hello_imgui/hello_imgui.h"


using VoidFunction = std::function<void(void)>;

class WindowWithEditor
{
public:
    WindowWithEditor(const std::string & windowLabel);

    void setEditorAnnotatedSource(const SourceParse::AnnotatedSource &annotatedSource);
    void RenderEditor(const std::string& filename, VoidFunction additionalGui = {});

    void searchForFirstOccurence(const std::string& search);
    static void searchForFirstOccurenceAndFocusWindow(
        const std::string& search,
        const std::string& windowName
    );

    std::string windowLabel() const { return mWindowLabel; }

    TextEditor * _GetTextEditorPtr() { return &mEditor; }
    TextEditor& InnerTextEditor() { return mEditor; }

private:
    void guiStatusLine(const std::string& filename);
    void guiFind();
    void guiIconBar(VoidFunction additionalGui);
    void editorContextMenu();
    #ifdef __EMSCRIPTEN__
    void handleJsClipboardShortcuts();
    #endif

protected:
    std::string mWindowLabel;
    TextEditor mEditor;
    ImGuiTextFilter mFilter;
    int mNbFindMatches = 0;
    bool mShowLongLinesOverlay = true;
};

void menuEditorTheme();
void LoadMonospaceFont();

