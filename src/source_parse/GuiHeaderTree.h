#pragma once
#include "source_parse/HeaderTree.h"
#include "imgui.h"

extern bool                     GDemoMarker_FlagFollowSource;

namespace SourceParse
{

    class GuiHeaderTree
    {
    public:
        GuiHeaderTree(const LinesWithTags & linesWithTags);

        // Show a tree gui with all the tags
        // return a line number if the user selected a tag, returns -1 otherwise
        virtual int gui(int currentEditorLineNumber);

        void setShowToc(bool v) { mShowToc = v; }

    protected:
        int guiImpl(int currentEditorLineNumber, const HeaderTree& headerTree, bool isRootNode);
        void applyTocFilter();
        void showExpandCollapseButtons();
        void showCommandLine();

        HeaderTree mHeaderTree;
        HeaderTree mFilteredHeaderTree;
        ImGuiTextFilter mFilter;
        bool mShowToc = true;
        bool mScrollToSelectedNextTime = false; // only valid for "follow" mode (in subclass)

        enum class ExpandCollapseAction
        {
            ExpandAll,
            CollapseAll,
            NoAction
        };
        ExpandCollapseAction mExpandCollapseAction = ExpandCollapseAction::CollapseAll;
    };


    class GuiHeaderTree_FollowDemo: public GuiHeaderTree
    {
    public:
        GuiHeaderTree_FollowDemo(const LinesWithTags & linesWithTags) : GuiHeaderTree(linesWithTags) {
            GDemoMarker_FlagFollowSource = true;
        }

        int gui(int currentEditorLineNumber) override
        {
            ImGui::SameLine();
            return GuiHeaderTree::gui(GDemoMarker_FlagFollowSource ? mCurrentFollowedLine : currentEditorLineNumber);
        }

        void followShowTocElementForLine(int sourceLineNumber)
        {
            mCurrentFollowedLine = sourceLineNumber;
            mScrollToSelectedNextTime = true;
        }

    private:
        int mCurrentFollowedLine = -1;
    };


} // namespace SourceParse


