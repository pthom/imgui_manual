#pragma once
#include "source_parse/Sources.h"
#include "TextEditor.h"
#include "imgui.h"


using VoidFunction = std::function<void(void)>;

class WindowWithEditor
{
public:
    WindowWithEditor();

    void setEditorAnnotatedSource(const SourceParse::AnnotatedSource &annotatedSource);
    void RenderEditor(const std::string& filename, VoidFunction additionalGui = {});

    TextEditor * _GetTextEditorPtr() { return &mEditor; }

private:
    void guiStatusLine(const std::string& filename);
    void guiFind();
    void guiIconBar(VoidFunction additionalGui);

protected:
    TextEditor mEditor;
    ImGuiTextFilter mFilter;
    int mNbFindMatches = 0;
    bool mShowLongLinesOverlay = true;
};

void menuEditorTheme();
void LoadMonospaceFont();
