#pragma once
#include "source_parse/HeaderTree.h"
#include "imgui.h"

namespace SourceParse
{

    class GuiHeaderTree
    {
    public:
        GuiHeaderTree(const LinesWithTags & linesWithTags);

        // Show a tree gui with all the tags
        // return a line number if the user selected a tag, returns -1 otherwise
        int gui(int currentEditorLineNumber);
    private:
        int guiImpl(int currentEditorLineNumber, const HeaderTree& headerTree, bool isRootNode = false);
        void applyTocFilter();
        void showExpandCollapseButtons();
        void showCommandLine();

        HeaderTree mHeaderTree;
        HeaderTree mFilteredHeaderTree;
        ImGuiTextFilter mFilter;
        bool mShowToc = true;

        enum class ExpandCollapseAction
        {
            ExpandAll,
            CollapseAll,
            NoAction
        };
        ExpandCollapseAction mExpandCollapseAction = ExpandCollapseAction::NoAction;
    };


} // namespace SourceParse


