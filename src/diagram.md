View this with [markdown preview enhanced](https://shd101wyy.github.io/markdown-preview-enhanced/#/) or refer to diagram.png

````puml
@startuml
namespace ExternalLibs {
    class ImGuiColorTextEdit
    class Markdown
}


class WindowWithEditor {
    ---
    + RenderEditor()
    ---
    Members
    - TextEditor mEditor
}
WindowWithEditor *-- ExternalLibs.ImGuiColorTextEdit
WindowWithEditor *-- ExternalLibs.Markdown

class LibrariesCodeBrowser {
    + gui()
    - bool guiSelectLibrarySource()

    - vector<Library> mLibraries
    - mCurrentSource
}

LibrariesCodeBrowser --|> WindowWithEditor
LibrariesCodeBrowser *-- SourceParse.Library: vector
LibrariesCodeBrowser *-- SourceParse.Source

class ImGuiCodeBrowser {
    + ImGuiCodeBrowser()
    + gui()

    - mLibrariesCodeBrowser
}
ImGuiCodeBrowser *-- LibrariesCodeBrowser

class ImGuiCppDocBrowser {
    + gui()
    - mAnnotatedSource
}
ImGuiCppDocBrowser --|> WindowWithEditor
ImGuiCppDocBrowser *-- SourceParse.AnnotatedSource


class ImGuiDemoBrowser {
    + gui()

    - mAnnotatedSource
}

ImGuiDemoBrowser --|> WindowWithEditor
ImGuiDemoBrowser *-- SourceParse.AnnotatedSource

class Acknowledgments {
    + gui()
    - mLibrariesCodeBrowser
}

Acknowledgments *-- LibrariesCodeBrowser



class AboutWindow {
    + gui()
    - mLibrariesCodeBrowser
}

AboutWindow *-- LibrariesCodeBrowser



namespace SourceParse {
    class Source<struct> {
        sourcePath
        sourceCode
    }
    class LineWithTag<struct> {
        lineNumber
        tag
    }

    class AnnotatedSource<struct> {
        Source source
        LinesWithTags linesWithTags
    }
    AnnotatedSource *-- LineWithTag: vector
    AnnotatedSource *-- Source

    class Library<struct> {
        path
        name
        url
        shortDoc // markdown
        vector<string> sourcePaths
    }

}

class ImGuiManual<main> {
    - imGuiDemoBrowser
    - imGuiCppDocBrowser
    - imGuiCodeBrowser
    - acknowledgments
    - aboutWindow
}
ImGuiManual *-- ImGuiDemoBrowser
ImGuiManual *-- ImGuiCppDocBrowser
ImGuiManual *-- ImGuiCodeBrowser
ImGuiManual *-- ImGuiDemoBrowser
ImGuiManual *-- Acknowledgments
ImGuiManual *-- AboutWindow
ImGuiManual *-- HelloImGui.RunnerParams

namespace HelloImGui {
    class RunnerParams
    RunnerParams *-- RunnerCallbacks
    RunnerParams *-- DockingParams
    RunnerParams *-- WindowParams
}

@enduml
````

