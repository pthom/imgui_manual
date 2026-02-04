#include "source_parse/ImGuiRepoUrl.h"

#include "imgui_utilities/HyperlinkHelper.h"
#include "imgui_utilities/ImGuiExt.h"
#include "source_parse/ImGuiDemoParser.h"

#include "hello_imgui/hello_imgui.h"
#include "imgui_demo_marker_hooks.h"

#include <fplus/fplus.hpp>

#include "ImGuiDemoBrowser.h"


// implImGuiDemoCallbackDemoCallback is the implementation
// of imgui_demo.cpp's global callback (gImGuiDemoCallback)
// And  gImGuiDemoBrowser is a global reference to the browser used by this callback
ImGuiDemoBrowser *gImGuiDemoBrowser = nullptr;
extern HelloImGui::RunnerParams runnerParams; // defined in ImGuiManual.cpp
void implImGuiDemoCallbackDemoCallback(const char* file, int line, const char* section)
{
    gImGuiDemoBrowser->ImGuiDemoCallback(file, line, section);
}


ImGuiDemoBrowser::ImGuiDemoBrowser():
    mSourceElementsCpp(SourceParse::ReadImGuiDemoCode(), "imgui_demo.cpp"),
    mSourceElementsPython(SourceParse::ReadImGuiDemoCodePython(), "imgui_demo.py")
{
    // Setup of imgui_demo.cpp's global callback
    // (GImGuiDemoMarkerCallback belongs to imgui.cpp!)
    GImGuiDemoMarkerCallback = implImGuiDemoCallbackDemoCallback;

    gImGuiDemoBrowser = this;
}


std::optional<SourceParse::LineWithTag> findLineWithOriginalTag(const SourceParse::AnnotatedSource& as, const std::string& tag)
{
    auto matchingTags = fplus::keep_if(
        [&tag](const SourceParse::LineWithTag& lt) { return lt._original_tag_full == tag; },
        as.linesWithTags
    );
    if (matchingTags.size() == 1)
        return matchingTags[0];
    else
        return std::nullopt;
}

void ImGuiDemoBrowser::ImGuiDemoCallback(const char* /*file*/, int /*line_number*/, const char* tag)
{
    if (strcmp(tag, "Menu") == 0)
        return;
    for (auto sourceElements: AllSourceElements())
    {
        auto lineWithTag = findLineWithOriginalTag(sourceElements->AnnotatedSource, tag);
        if (!lineWithTag.has_value())
            continue ;

        int line_number = lineWithTag->lineNumber + 1;
        int cursorLineOnPage = 3;
        sourceElements->mGuiHeaderTree.followShowTocElementForLine(line_number);
        sourceElements->mWindowWithEditor.InnerTextEditor().SetCursorPosition({line_number, 0}, cursorLineOnPage);
    }
}

void ImGuiDemoBrowser::gui()
{
    guiHelp();
    guiDemoCodeTags();
    guiSave();

    CurrentSourceElements().mWindowWithEditor.RenderEditor("imgui_demo", [this] { this->guiEditorAdditional(); });
}


void ImGuiDemoBrowser::guiHelp()
{
    std::string help =
        "This is the code of imgui_demo.cpp. It is the best way to learn about Dear ImGui! \n"
        "\n"
        "* On the left, you can see a demo that showcases all the widgets and features of ImGui:\n"
        "  If 'Code Lookup' is checked, the code editor and the the table of content will follow\n"
        "  the mouse whenever you hover a demo.\n"
        "\n"
        "* Below, the table of content (TOC) shows all the available demos: click on any item to see its code.\n"
        "  Alternatively, you can also search for some features (try searching for \"widgets\", \"layout\", \"drag\", etc)\n"
        ;
    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.f, 1.0f), "(?)");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("%s", help.c_str());
    ImGui::SameLine(50.f);
}

void ImGuiDemoBrowser::guiSave()
{
#ifdef IMGUI_MANUAL_CAN_WRITE_IMGUI_DEMO_CPP
    if (mViewPythonOrCpp == ViewPythonOrCpp::Cpp)
    {
        if (ImGui::Button("Save"))
        {
            auto& currentEditor = CurrentSourceElements().mWindowWithEditor.InnerTextEditor();
            std::string fileSrc = IMGUI_MANUAL_REPO_DIR "/external/imgui/imgui_demo.cpp";
            fplus::write_text_file(fileSrc, currentEditor.GetText())();
        }
    }
#endif
}

void ImGuiDemoBrowser::guiEditorAdditional()
{
    if (ImGui::SmallButton("View on github at this line"))
    {
        auto& currentEditor = CurrentSourceElements().mWindowWithEditor.InnerTextEditor();
        int lineNumber = currentEditor.GetCursorPosition().mLine + 1;

        std::string demoFileUrl;
        if (mViewPythonOrCpp == ViewPythonOrCpp::Cpp)
            demoFileUrl = ImGuiRepoUrl() + "imgui_demo.cpp";
        else
            demoFileUrl = "https://github.com/pthom/imgui_manual/blob/master/src/imgui_demo_python/imgui_demo.py";

        std::string url = demoFileUrl + "#L" + std::to_string(lineNumber);
        HyperlinkHelper::OpenUrl(url);
    }

    ImGui::SameLine();
    {
        bool checked = (mViewPythonOrCpp == ViewPythonOrCpp::Python);
        bool changed = ImGui::Checkbox("Python", &checked);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
            ImGui::SetTooltip("View python demo code, using \"Dear ImGui Bundle\" bindings:  https://pthom.github.io/imgui_bundle/");
        if (changed)
            mViewPythonOrCpp = checked ? ViewPythonOrCpp::Python : ViewPythonOrCpp::Cpp;
    }
}

void ImGuiDemoBrowser::guiDemoCodeTags()
{
    auto& currentEditor = CurrentSourceElements().mWindowWithEditor.InnerTextEditor();
    auto& currentGuiHeaderTree = CurrentSourceElements().mGuiHeaderTree;

    int currentEditorLineNumber = currentEditor.GetCursorPosition().mLine;
    int selectedLine = currentGuiHeaderTree.gui(currentEditorLineNumber);
    if (selectedLine >= 0)
        currentEditor.SetCursorPosition({selectedLine, 0}, 3);
}
