#include "imgui_utilities/ImGuiExt.h"
#include "hello_imgui/hello_imgui.h"
#include "hello_imgui/icons_font_awesome_4.h"
#include <fplus/fplus.hpp>
#include <map>
#include "WindowWithEditor.h"
#include "JsClipboardTricks.h"

std::vector<TextEditor *> gAllEditors;
std::vector<WindowWithEditor *> gAllWindowWithEditors;

ImFont * gMonospaceFont = nullptr;

void LoadMonospaceFont()
{
    float fontSize = 15.f;
    std::string fontFilename = "fonts/Inconsolata-Regular.ttf";
    gMonospaceFont = HelloImGui::LoadFontTTF(fontFilename, fontSize);
    // MergeFontAwesomeToLastFont(fontSize);
}

WindowWithEditor::WindowWithEditor(const std::string& windowLabel)
    : mWindowLabel(windowLabel)
{
    mEditor.SetPalette(TextEditor::GetLightPalette());
    mEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
    mEditor.SetReadOnly(true);
    mEditor.SetShowWhitespaces(false);
    gAllEditors.push_back(&mEditor);
    gAllWindowWithEditors.push_back(this);
}

void WindowWithEditor::setEditorAnnotatedSource(const SourceParse::AnnotatedSource &annotatedSource)
{
    mEditor.SetText(annotatedSource.source.sourceCode);
    std::unordered_set<int> lineNumbers;
    for (auto line : annotatedSource.linesWithTags)
        lineNumbers.insert(line.lineNumber + 1);
    mEditor.SetBreakpoints(lineNumbers);
}



void WindowWithEditor::RenderEditor(const std::string &filename, VoidFunction additionalGui)
{
    guiIconBar(additionalGui);
    guiStatusLine(filename);

    ImGui::PushFont(gMonospaceFont);
    mEditor.Render(filename.c_str());

    bool lineTooLong = false;
    {
        float editorWidth = ImGui::GetItemRectSize().x;
        float textWidth = ImGui::CalcTextSize(mEditor.GetCurrentLineText().c_str()).x + ImGui::GetFontSize() * 4.f;
        lineTooLong = textWidth > editorWidth;
    }

    ImGui::PopFont();

    editorContextMenu();
}

void WindowWithEditor::editorContextMenu()
{
    if (mEditor.GetSelectedText().empty())
        return;
    if (ImGui::BeginPopupContextItem("item context menu"))
    {
        std::map<std::string, std::string> fileAndEditorWindowName
            {
                {"imgui.h", "imgui.h - Doc"},
                // {"imgui.cpp", "imgui.cpp - Doc"},
                {"imgui_demo.cpp", "ImGui - Demo Code"}
            };

        std::string selectionShort = mEditor.GetSelectedText();
        if (selectionShort.size() > 30)
            selectionShort = fplus::take(30, selectionShort) + "...";

        for (const auto& kv: fileAndEditorWindowName)
        {
            std::string label = "Search for \"" + selectionShort + "\" in " + kv.first;
            if (ImGui::Selectable(label.c_str()))
                WindowWithEditor::searchForFirstOccurenceAndFocusWindow(
                    mEditor.GetSelectedText(), kv.second);
        }
        ImGui::EndPopup();
    }
}

void WindowWithEditor::guiStatusLine(const std::string &filename)
{
    auto & editor = mEditor;
    auto cpos = editor.GetCursorPosition();
    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
                editor.IsOverwrite() ? "Ovr" : "Ins",
                editor.CanUndo() ? "*" : " ",
                filename.c_str());
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
            ImGui::SetTooltip("Filter using -exc,inc. For example search for '-widgets,DEMO_MARKER'");
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
                mEditor.SetCursorPosition({(int)line_idx.unsafe_get_just(), 0}, 3);
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
                mEditor.SetCursorPosition({(int)line_idx.unsafe_get_just() + currentLine + 1, 0}, 3);
        }
    }

    ImGui::SameLine();
}

void WindowWithEditor::guiIconBar(VoidFunction additionalGui)
{
    auto & editor = mEditor;

    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_COPY, editor.HasSelection(), "Copy", true))
    {
        editor.Copy();
    }

    // #ifdef __EMSCRIPTEN__
    // ImGui::TextDisabled("?");
    // if (ImGui::IsItemHovered())
    //     ImGui::SetTooltip("Due to browser security limitations, this app will not communicate \n"
    //                       "with your computer's clipboard\n"
    //                       "Use the \"view on github\" button if you want to copy-paste \n"
    //                       "to an external IDE or application");
    // ImGui::SameLine();
    // #endif

    guiFind();

    if (additionalGui)
        additionalGui();

    ImGui::NewLine();
}

void WindowWithEditor::searchForFirstOccurence(const std::string& search)
{
    snprintf(mFilter.InputBuf, 256, "%s", search.c_str());
    mFilter.Build();
    const auto & lines = mEditor.GetTextLines();
    mNbFindMatches = (int)fplus::count_if([this](auto s){ return mFilter.PassFilter(s.c_str());}, lines);

    const auto &linesToSearch = mEditor.GetTextLines();
    auto line_idx = fplus::find_first_idx_by(
        [this](const std::string &line) {
          return mFilter.PassFilter(line.c_str());
        },
        linesToSearch);
    if (line_idx.is_just())
        mEditor.SetCursorPosition({(int)line_idx.unsafe_get_just(), 0}, 3);
}

extern HelloImGui::RunnerParams runnerParams; // defined in ImGuiManual.cpp

void WindowWithEditor::searchForFirstOccurenceAndFocusWindow(
    const std::string& search,
    const std::string& windowName
)
{
    for (auto windowWithEditor: gAllWindowWithEditors)
    {
        if (windowWithEditor->windowLabel() == windowName)
        {
            windowWithEditor->searchForFirstOccurence(search);
            runnerParams.dockingParams.focusDockableWindow(windowName);
        }
    }

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
