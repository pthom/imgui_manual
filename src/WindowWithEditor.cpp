#include "hello_imgui.h"
#include <fplus/fplus.hpp>
#include "utilities/ImGuiExt.h"
#include "WindowWithEditor.h"

std::vector<TextEditor *> gAllEditors;

WindowWithEditor::WindowWithEditor()
{
    mEditor.SetPalette(TextEditor::GetLightPalette());
    mEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
    mEditor.SetReadOnly(true);
    gAllEditors.push_back(&mEditor);
}

void WindowWithEditor::setEditorAnnotatedSource(const Sources::AnnotatedSource &annotatedSource)
{
    mEditor.SetText(annotatedSource.source.sourceCode);
    std::unordered_set<int> lineNumbers;
    for (auto line : annotatedSource.linesWithTags)
        lineNumbers.insert(line.lineNumber);
    mEditor.SetBreakpoints(lineNumbers);
}
void WindowWithEditor::RenderEditor(const std::string &filename, VoidFunction additionalGui)
{
    guiIconBar(additionalGui);
    guiStatusLine(filename);
    mEditor.Render(filename.c_str());
}

void WindowWithEditor::guiStatusLine(const std::string &filename)
{
    auto & editor = mEditor;
    auto cpos = editor.GetCursorPosition();
    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
                editor.IsOverwrite() ? "Ovr" : "Ins",
                editor.CanUndo() ? "*" : " ",
                editor.GetLanguageDefinition().mName.c_str(), filename.c_str());
}

void WindowWithEditor::guiFind()
{
    ImGui::SameLine();
    // Draw filter
    bool filterChanged = false;
    {
        ImGui::SetNextItemWidth(100.f);
        filterChanged = mFilter.Draw("Search code"); ImGui::SameLine();
        ImGui::SameLine();
        ImGui::TextDisabled("?");
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Filter using -exc,inc. For example search for '-widgets,DemoCode'");
        ImGui::SameLine();
    }
    // If changed, check number of matches
    if (filterChanged)
    {
        const auto & lines = mEditor.GetTextLines();
        mNbFindMatches = (int)fplus::count_if([this](auto s){ return mFilter.PassFilter(s.c_str());}, lines);
    }

    // Draw number of matches
    {
        if (mNbFindMatches > 0)
        {
            const auto & lines = mEditor.GetTextLines();
            bool thisLineMatch = mFilter.PassFilter(mEditor.GetCurrentLineText().c_str());
            if (!thisLineMatch)
                ImGui::Text("---/%3i", mNbFindMatches);
            else
            {
                std::vector<size_t> allMatchingLinesNumbers =
                    fplus::find_all_idxs_by(
                        [this](const std::string& s) {
                          return mFilter.PassFilter(s.c_str());
                        },
                        lines);
                auto matchNumber = fplus::find_first_idx(
                    (size_t)mEditor.GetCursorPosition().mLine,
                    allMatchingLinesNumbers);
                if (matchNumber.is_just())
                    ImGui::Text("%3i/%3i", (int)matchNumber.unsafe_get_just() + 1, mNbFindMatches);
                else
                    ImGui::Text("Houston, we have a bug");
                ImGui::SameLine();
            }
            ImGui::SameLine();
        }
        ImGui::SameLine();
    }

    // Perform search down or up
    {
        bool searchDown = ImGui::SmallButton(ICON_FA_ARROW_DOWN); ImGui::SameLine();
        bool searchUp = ImGui::SmallButton(ICON_FA_ARROW_UP); ImGui::SameLine();
        std::vector<std::string> linesToSearch;
        int currentLine = mEditor.GetCursorPosition().mLine ;
        if (searchUp)
        {
            const auto & lines = mEditor.GetTextLines();
            linesToSearch = fplus::get_segment(0, currentLine, lines);
            auto line_idx = fplus::find_last_idx_by(
                [this](const std::string &line) {
                  return mFilter.PassFilter(line.c_str());
                },
                linesToSearch);
            if (line_idx.is_just())
                mEditor.SetCursorPosition({(int)line_idx.unsafe_get_just(), 0});
        }
        if (searchDown)
        {
            const auto &lines = mEditor.GetTextLines();
            linesToSearch = fplus::get_segment(currentLine + 1, lines.size(), lines);
            auto line_idx = fplus::find_first_idx_by(
                [this](const std::string &line) {
                  return mFilter.PassFilter(line.c_str());
                },
                linesToSearch);
            if (line_idx.is_just())
                mEditor.SetCursorPosition({(int)line_idx.unsafe_get_just() + currentLine + 1, 0});
        }
    }

    ImGui::SameLine();
}

void WindowWithEditor::guiIconBar(VoidFunction additionalGui)
{
    auto & editor = mEditor;
    static bool canWrite = ! editor.IsReadOnly();
    if (ImGui::Checkbox(ICON_FA_EDIT, &canWrite))
        editor.SetReadOnly(!canWrite);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Enable editing this file");
    ImGui::SameLine();
    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_UNDO, editor.CanUndo() && canWrite, "Undo", true))
        editor.Undo();
    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_REDO, editor.CanRedo() && canWrite, "Redo", true))
        editor.Redo();
    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_COPY, editor.HasSelection(), "Copy", true))
        editor.Copy();
    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_CUT, editor.HasSelection() && canWrite, "Cut", true))
        editor.Cut();
    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_PASTE, (ImGui::GetClipboardText() != nullptr)  && canWrite, "Paste", true))
        editor.Paste();

    // missing icon from font awesome
    // if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_SELECT_ALL, ImGui::GetClipboardText() != nullptr, true))
    //      editor.PASTE();

    #ifdef __EMSCRIPTEN__
    ImGui::TextDisabled("?");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Due to browser security limitations, this app will not communicate \n"
                          "with your computer's clipboard\n"
                          "Use the \"view on github\" button if you want to copy-paste \n"
                          "to an external IDE or application");
    ImGui::SameLine();
    #endif

    guiFind();

    if (additionalGui)
        additionalGui();

    ImGui::NewLine();
}

void menuEditorTheme()
{
    ImGui::MenuItem("Editor", NULL, false, false);
    if (ImGui::MenuItem("Dark palette"))
        for (auto editor: gAllEditors)
            editor->SetPalette(TextEditor::GetDarkPalette());
    if (ImGui::MenuItem("Light palette"))
        for (auto editor: gAllEditors)
            editor->SetPalette(TextEditor::GetLightPalette());
    if (ImGui::MenuItem("Retro blue palette"))
        for (auto editor: gAllEditors)
            editor->SetPalette(TextEditor::GetRetroBluePalette());
}
