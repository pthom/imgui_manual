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
        ExpandCollapseAction mExpandCollapseAction = ExpandCollapseAction::NoAction;
    };


    class GuiHeaderTree_FollowDemo: public GuiHeaderTree
    {
    public:
        GuiHeaderTree_FollowDemo(const LinesWithTags & linesWithTags) : GuiHeaderTree(linesWithTags) {
            setIsFollowActive(true);
        }

        int gui(int currentEditorLineNumber) override
        {
            bool follow = mIsFollowActive;
            bool changed = ImGui::Checkbox("Follow Demo", &follow);
            ImGui::SameLine();
            if (changed)
                setIsFollowActive(follow);
            return GuiHeaderTree::gui(mIsFollowActive ? mCurrentFollowedLine : currentEditorLineNumber);
        }

        void setIsFollowActive(bool follow)
        {
            mIsFollowActive = follow;
            if (follow)
                mExpandCollapseAction = ExpandCollapseAction::ExpandAll;
        }

        void followShowTocElementForLine(int sourceLineNumber)
        {
            if (!mIsFollowActive)
                return;
            mCurrentFollowedLine = sourceLineNumber;
            mScrollToSelectedNextTime = true;
        }

        bool isFollowActive()
        {
            return mIsFollowActive;
        }

    private:
        bool mIsFollowActive = true;
        int mCurrentFollowedLine = -1;
    };


} // namespace SourceParse


