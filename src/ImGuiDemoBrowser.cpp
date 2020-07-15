#include "sources/ImGuiRepoUrl.h"

#include "utilities/HyperlinkHelper.h"
#include "utilities/ImGuiExt.h"
#include "utilities/MarkdownHelper.h"

#include "hello_imgui/hello_imgui.h"

#include "ImGuiDemoBrowser.h"


// implImGuiDemoCallbackDemoCallback is the implementation
// of imgui_demo.cpp's global callback (gImGuiDemoCallback)
// And  gEditorImGuiDemo is a global reference to the text editor
// used by this callback
TextEditor *gEditorImGuiDemo = nullptr;
void implImGuiDemoCallbackDemoCallback(int line_number)
{
    int cursorLineOnPage = 3;
    gEditorImGuiDemo->SetCursorPosition({line_number, 0}, cursorLineOnPage);
}


ImGuiDemoBrowser::ImGuiDemoBrowser()
    : WindowWithEditor()
    , mAnnotatedSource(Sources::ReadImGuiDemoCode())
{
    setEditorAnnotatedSource(mAnnotatedSource);

    // Setup of imgui_demo.cpp's global callback
    // (gImGuiDemoCallback belongs to imgui.cpp!)
    gEditorImGuiDemo = _GetTextEditorPtr();
    gImGuiDemoCallback = implImGuiDemoCallbackDemoCallback;
}

void ImGuiDemoBrowser::gui()
{
    guiHelp();
    guiDemoCodeTags();
    guiSave();
    RenderEditor("imgui_demo.cpp", [this] { this->guiGithubButton(); });
}

void ImGuiDemoBrowser::guiHelp()
{
    static bool showHelp = true;
    if (showHelp)
    {
        std::string help =
            "This is the code of imgui_demo.cpp. It is the best way to learn about Dear ImGui! \n"
            "On the left, you can see a demo that showcases all the widgets and features of ImGui: "
            "Click on the \"Code\" buttons to see their code and learn about them. \n"
            "Alternatively, you can also search for some features (try searching for \"widgets\", \"layout\", \"drag\", etc)";
        ImGui::TextWrapped("%s", help.c_str());
        //ImGui::SameLine();
        if (ImGui::Button(ICON_FA_THUMBS_UP " Got it"))
        showHelp = false;
    }
}

void ImGuiDemoBrowser::guiSave()
{
#ifdef IMGUI_MANUAL_CAN_WRITE_IMGUI_DEMO_CPP
    if (ImGui::Button("Save"))
        {
            std::string fileSrc = IMGUI_MANUAL_REPO_DIR "/external/imgui/imgui_demo.cpp";
            fplus::write_text_file(fileSrc, mEditor.GetText())();
        }
#endif
}

void ImGuiDemoBrowser::guiGithubButton()
{
    if (ImGui::SmallButton("View on github at this line"))
    {
        std::string url = ImGuiRepoUrl() + "imgui_demo.cpp#L"
                          + std::to_string(mEditor.GetCursorPosition().mLine);
        HyperlinkHelper::OpenUrl(url);
    }
}

void ImGuiDemoBrowser::guiDemoCodeTags()
{
    bool showTooltip = false;
    ImGui::Text("Search demos"); ImGui::SameLine();
    if (ImGui::IsItemHovered())
        showTooltip = true;
    ImGui::TextDisabled("?"); ImGui::SameLine();
    if (ImGui::IsItemHovered())
        showTooltip = true;
    if (showTooltip)
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(
            "Filter usage:[-excl],incl\n"
            "For example:\n"
            "   \"button\" will search for \"button\"\n"
            "   \"-widget,button\" will search for \"button\" without \"widget\""
        );
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

    static ImGuiTextFilter filter;
    filter.Draw();
    if (strlen(filter.InputBuf) >= 3)
    {
        for (const auto & lineWithNote : mAnnotatedSource.linesWithTags)
        {
            if (filter.PassFilter(lineWithNote.tag.c_str()))
            {
                if (ImGui::SmallButton(lineWithNote.tag.c_str()))
                    mEditor.SetCursorPosition({lineWithNote.lineNumber, 0}, 3);
                ImGuiExt::SameLine_IfPossible(150.f);
            }
        }
        ImGui::NewLine();
    }
}
