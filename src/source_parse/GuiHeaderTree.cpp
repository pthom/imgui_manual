#include "imgui.h"
#include "GuiHeaderTree.h"

namespace SourceParse
{

GuiHeaderTree::GuiHeaderTree(const LinesWithTags & linesWithTags)
{
    mHeaderTree = makeHeaderTree(linesWithTags);
}

ImGuiTreeNodeFlags makeTreeNodeFlags(bool isLeafNode, bool isSelected)
{
    ImGuiTreeNodeFlags treeNodeFlags;

    static ImGuiTreeNodeFlags baseTreeFlags =
        ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_OpenOnDoubleClick
        | ImGuiTreeNodeFlags_SpanAvailWidth;

    static ImGuiTreeNodeFlags leafFlags = baseTreeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    treeNodeFlags = isLeafNode ? leafFlags : baseTreeFlags;

    if (isSelected)
        treeNodeFlags |= ImGuiTreeNodeFlags_Selected;

    return treeNodeFlags;
}

int GuiHeaderTree::guiImpl(int currentEditorLineNumber, const HeaderTree& headerTree)
{
    using namespace std::literals;

    const auto &lineWithTag = headerTree.value_;
    int clickedLineNumber = -1;

    bool isLeafNode = headerTree.children_.empty();
    bool isSelected = false;
    {
        int diffLine = currentEditorLineNumber - lineWithTag.lineNumber;
        if ((diffLine >= 0) && (diffLine < 5))
            isSelected = true;
    }

    ImGuiTreeNodeFlags treeNodeFlags = makeTreeNodeFlags(isLeafNode, isSelected);

    std::string title = lineWithTag.tag
                        + "##" + std::to_string(lineWithTag.lineNumber);
    bool node_open = ImGui::TreeNodeEx(title.c_str(), treeNodeFlags);
    if (ImGui::IsItemClicked())
        clickedLineNumber = lineWithTag.lineNumber;

    if (node_open && !isLeafNode)
    {
        auto showChildrenNodes = [this, headerTree, currentEditorLineNumber]()
        {
          int clickedLineNumber_Child = -1;
          for(const auto& headerTreeChild: headerTree.children_)
          {
              int line = guiImpl(currentEditorLineNumber, headerTreeChild);
              if (line > 0)
                  clickedLineNumber_Child = line;
          }
          return clickedLineNumber_Child;
        };

        int selectedChildLine = showChildrenNodes();
        if (selectedChildLine > 0)
            clickedLineNumber = selectedChildLine;
        ImGui::TreePop();
    }
    return clickedLineNumber;
}


// Show a tree gui with all the tags
// return a line number if the user selected a tag, returns -1 otherwise
int GuiHeaderTree::gui(int currentEditorLineNumber)
{
    return guiImpl(currentEditorLineNumber, mHeaderTree);
}



} // namespace SourceParse