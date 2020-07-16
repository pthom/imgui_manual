#pragma once
#include "source_parse/HeaderTree.h"

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
        int guiImpl(int currentEditorLineNumber, const HeaderTree& headerTree);

        HeaderTree mHeaderTree;
    };


} // namespace SourceParse


