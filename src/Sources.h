#pragma once
#include <vector>
#include <string>
#include <map>

namespace Sources
{
    struct Source
    {
        std::string sourcePath;
        std::string sourceCode;
    };

    struct LineWithTag
    {
        int lineNumber;
        std::string tag;
    };
    using LinesWithTags = std::vector<LineWithTag>;

    struct AnnotatedSourceCode
    {
        Source source;
        LinesWithTags linesWithTags;
    };

    struct Library
    {
        std::string path;
        std::string name;
        std::string url;
        std::vector<std::string> sourcePaths;
    };

    std::vector<Library> imguiLibrary();
    std::vector<Library> thisDemoLibraries();
    std::vector<Library> otherLibraries();

    Source ReadSource(const std::string sourcePath);
    AnnotatedSourceCode ReadImGuiDemoCode(const std::string& sourcePath);
}
