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
    float fontSize = 14.f;
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

void RenderLongLinesOverlay(const std::string& currentCodeLine)
{
    using namespace std::literals;

    std::string code, comment;
    {
        auto commentPosition = fplus::find_first_instance_of_token("//"s, currentCodeLine);
        if (commentPosition.is_nothing())
            code = currentCodeLine;
        else
        {
            code = fplus::trim_whitespace(fplus::take(commentPosition.unsafe_get_just(), currentCodeLine));
            comment = fplus::drop(commentPosition.unsafe_get_just() + 2, currentCodeLine);
        }
    }

    ImGui::SetNextWindowBgAlpha(0.75f); // Transparent background
    ImGuiWindowFlags window_flags =
          ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoNav;

    ImVec2 commentOverlaySize(ImVec2(ImGui::GetWindowWidth(), 60.f));
    ImGui::SetNextWindowSize( commentOverlaySize, ImGuiCond_Always );
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - commentOverlaySize.x,
               ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - commentOverlaySize.y),
        ImGuiCond_Always
        );

    if (ImGui::Begin("Comment Overlay", nullptr, window_flags))
    {
        if (!code.empty())
        {
            ImGui::PushFont(gMonospaceFont);
            ImGui::TextWrapped("%s", code.c_str());
            ImGui::PopFont();
        }
        if (!comment.empty())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 1.f, 0.6f, 1.f));
            ImGui::TextWrapped("%s", comment.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::End();
    }
}

#ifdef __EMSCRIPTEN__
void WindowWithEditor::handleJsClipboardShortcuts()
{
  //if (!ImGui::IsItemFocused())
  //    return;

  ImGuiIO& io = ImGui::GetIO();
  auto shift = io.KeyShift;
  auto ctrl = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl;
  auto alt = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeyAlt;

  bool shallFillBrowserClipboard = false;
  if (ctrl && !shift && !alt && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Insert)))
      shallFillBrowserClipboard = true;
  else if (ctrl && !shift && !alt && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_C)))
      shallFillBrowserClipboard = true;
  else if (ctrl && !shift && !alt && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X)))
      shallFillBrowserClipboard = true;
  else if (!ctrl && shift && !alt && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
      shallFillBrowserClipboard = true;

  #ifdef IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER
  if (shallFillBrowserClipboard)
      JsClipboard_SetClipboardText(mEditor.GetSelectedText().c_str());
  #endif
}
#endif // #ifdef __EMSCRIPTEN__

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
    if (mShowLongLinesOverlay && lineTooLong)
        RenderLongLinesOverlay(mEditor.GetCurrentLineText());

#ifdef __EMSCRIPTEN__
    handleJsClipboardShortcuts();
#endif
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
    static bool canWrite = ! editor.IsReadOnly();
    if (ImGui::Checkbox(ICON_FA_EDIT, &canWrite))
        editor.SetReadOnly(!canWrite);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Enable editing this file");
    ImGui::SameLine();

    ImGui::Checkbox(ICON_FA_BOOK, &mShowLongLinesOverlay);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Display wrapped line below the editor");

    ImGui::SameLine();

    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_UNDO, editor.CanUndo() && canWrite, "Undo", true))
        editor.Undo();
    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_REDO, editor.CanRedo() && canWrite, "Redo", true))
        editor.Redo();
    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_COPY, editor.HasSelection(), "Copy", true))
    {
        editor.Copy();
        #ifdef IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER
        JsClipboard_SetClipboardText(ImGui::GetClipboardText());
        #endif
    }
    if (ImGuiExt::SmallButton_WithEnabledFlag(ICON_FA_CUT, editor.HasSelection() && canWrite, "Cut", true))
    {
      editor.Cut();
      #ifdef IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER
      JsClipboard_SetClipboardText(ImGui::GetClipboardText());
      #endif
    }
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
