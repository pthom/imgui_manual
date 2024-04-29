#include "hello_imgui/hello_imgui.h"
#include "hello_imgui/icons_font_awesome_4.h"
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

int GuiHeaderTree::guiImpl(int currentEditorLineNumber, const HeaderTree& headerTree, bool isRootNode)
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
    if (mScrollToSelectedNextTime && isSelected)
    {
        ImGui::SetScrollHereY();
        mScrollToSelectedNextTime = false;
    }

    ImGuiTreeNodeFlags treeNodeFlags = makeTreeNodeFlags(isLeafNode, isSelected);

    std::string title = lineWithTag.tag
                        + "##" + std::to_string(lineWithTag.lineNumber);

    if (mExpandCollapseAction == ExpandCollapseAction::CollapseAll)
        ImGui::SetNextItemOpen(false, ImGuiCond_Always);
    if (mExpandCollapseAction == ExpandCollapseAction::ExpandAll)
        ImGui::SetNextItemOpen(true, ImGuiCond_Always);

    bool isNodeOpen;
    if (isRootNode)
        isNodeOpen = true;
    else
    {
        isNodeOpen = ImGui::TreeNodeEx(title.c_str(), treeNodeFlags);
        if (ImGui::IsItemClicked())
            clickedLineNumber = lineWithTag.lineNumber;
    }

    if (isNodeOpen && !isLeafNode)
    {
        auto showChildrenNodes = [this, &headerTree, currentEditorLineNumber]()
        {
          int clickedLineNumber_Child = -1;
          for(const auto& headerTreeChild: headerTree.children_)
          {
              int line = guiImpl(currentEditorLineNumber, headerTreeChild, false);
              if (line > 0)
                  clickedLineNumber_Child = line;
          }
          return clickedLineNumber_Child;
        };

        int selectedChildLine = showChildrenNodes();
        if (selectedChildLine > 0)
            clickedLineNumber = selectedChildLine;

        if (!isRootNode)
            ImGui::TreePop();
    }
    return clickedLineNumber;
}

bool DrawImGuiTextFilterWithTooltip(
    ImGuiTextFilter &imGuiTextFilter,
    const std::string &tooltipText =
        "Filter usage:[-excl],incl\n"
        "For example:\n"
        "   \"button\" will search for \"button\"\n"
        "   \"-widget,button\" will search for \"button\" without \"widget\"",
    const std::string& filterLabel =
        "Filter usage:[-excl],incl"
    )
{
    bool showTooltip = false;
    ImGui::TextDisabled("?"); ImGui::SameLine();
    if (ImGui::IsItemHovered())
        showTooltip = true;
    if (showTooltip)
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(tooltipText.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    ImGui::SetNextItemWidth(200.f);
    return imGuiTextFilter.Draw(filterLabel.c_str());
}


// Show a tree gui with all the tags
// return a line number if the user selected a tag, returns -1 otherwise
int GuiHeaderTree::gui(int currentEditorLineNumber)
{
    ImGui::Checkbox("Show Table Of Content", &mShowToc);
    if (!mShowToc)
        return -1;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    bool border = true;
    ImVec2 guiSize(0, ImGui::GetWindowHeight() / 4.f);
    ImGui::BeginChild("ChildR", guiSize, border, window_flags);
        ImGui::BeginMenuBar();
            showCommandLine();
        ImGui::EndMenuBar();

        int lineNumber = guiImpl(currentEditorLineNumber, mFilteredHeaderTree, true);
    ImGui::EndChild();

    mExpandCollapseAction = ExpandCollapseAction::NoAction;

    return lineNumber;
}

void GuiHeaderTree::showCommandLine()
{
    ImGui::Text("Table Of Contents");
    ImGui::SameLine();
    if (DrawImGuiTextFilterWithTooltip(mFilter))
        applyTocFilter();
    ImGui::SameLine();
    showExpandCollapseButtons();
}

void GuiHeaderTree::showExpandCollapseButtons()
{
    if (ImGui::Button(ICON_FA_PLUS_SQUARE " Expand all"))
        mExpandCollapseAction = ExpandCollapseAction::ExpandAll;
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_MINUS_SQUARE " Collapse all"))
        mExpandCollapseAction = ExpandCollapseAction::CollapseAll;
}

void GuiHeaderTree::applyTocFilter()
{
    auto lambdaPassFilter = [this](const LineWithTag& t) {
      return mFilter.PassFilter(t.tag.c_str());
    };
    mFilteredHeaderTree = tree_keep_wholebranch_if(
        lambdaPassFilter, mHeaderTree);
}


} // namespace SourceParse