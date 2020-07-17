#include "hello_imgui/hello_imgui.h"
#include "GuiHeaderTree.h"

namespace SourceParse
{

GuiHeaderTree::GuiHeaderTree(const LinesWithTags & linesWithTags)
{
    mHeaderTree = makeHeaderTree(linesWithTags);
    mFilteredHeaderTree = mHeaderTree;
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

    if (mExpandCollapseAction == ExpandCollapseAction::CollapseAll)
        ImGui::SetNextItemOpen(false, ImGuiCond_Always);
    if (mExpandCollapseAction == ExpandCollapseAction::ExpandAll)
        ImGui::SetNextItemOpen(true, ImGuiCond_Always);
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
    ImGuiWindowFlags window_flags = 0;//ImGuiWindowFlags_None;
//    window_flags |= ImGuiWindowFlags_MenuBar;
//    window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    bool border = true;
    ImVec2 guiSize(0, ImGui::GetWindowHeight() / 3.f);
    ImGui::BeginChild("ChildR", guiSize, border, window_flags);

    bool filterChanged = mFilter.Draw();
    if (filterChanged)
    {
        auto lambdaPassFilter = [this](const LineWithTag& t) {
            return mFilter.PassFilter(t.tag.c_str());
        };
        mFilteredHeaderTree = SourceParse::treeKeepIfAnyAncesterMatches(
            lambdaPassFilter, mHeaderTree);
    }
    if (ImGui::Button(ICON_FA_PLUS_SQUARE "Expand all"))
        mExpandCollapseAction = ExpandCollapseAction::ExpandAll;
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_MINUS_SQUARE "Collapse all"))
        mExpandCollapseAction = ExpandCollapseAction::CollapseAll;

    int r = guiImpl(currentEditorLineNumber, mFilteredHeaderTree);
    mExpandCollapseAction = ExpandCollapseAction::NoAction;

    ImGui::EndChild();

    return r;
}



} // namespace SourceParse