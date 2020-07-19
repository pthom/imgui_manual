#include "source_parse/ImGuiRepoUrl.h"

#include "imgui_utilities/HyperlinkHelper.h"
#include "imgui_utilities/ImGuiExt.h"
#include "imgui_utilities/MarkdownHelper.h"
#include "source_parse/ImGuiDemoParser.h"

#include "hello_imgui/hello_imgui.h"

#include <fplus/fplus.hpp>

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
    : WindowWithEditor("ImGui - Demo Code")
    , mAnnotatedSource(SourceParse::ReadImGuiDemoCode())
    , mGuiHeaderTree(mAnnotatedSource.linesWithTags)
{
    mGuiHeaderTree.setShowToc(false);
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
                          + std::to_string(mEditor.GetCursorPosition().mLine + 1);
        HyperlinkHelper::OpenUrl(url);
    }
}

void ImGuiDemoBrowser::guiDemoCodeTags()
{
    int currentEditorLineNumber = mEditor.GetCursorPosition().mLine;
    int selectedLine = mGuiHeaderTree.gui(currentEditorLineNumber);
    if (selectedLine >= 0)
        mEditor.SetCursorPosition({selectedLine, 0}, 3);
}
