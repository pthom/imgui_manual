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
    int level = 0; // optional header level
};
using LinesWithTags = std::vector<LineWithTag>;

struct AnnotatedSource
{
    Source source;
    LinesWithTags linesWithTags;
};

struct Library
{
    std::string path;
    std::string name;
    std::string url;
    std::string shortDoc; // markdown
    std::vector<std::string> sourcePaths;
};

std::vector<Library> imguiLibrary();
std::vector<Library> helloImGuiLibrary();
std::vector<Library> imguiManualLibrary();
std::vector<Library> otherLibraries();
std::vector<Library> acknowldegmentLibraries();


Source ReadSource(const std::string sourcePath);
AnnotatedSource ReadImGuiDemoCode();
AnnotatedSource ReadImGuiCppDoc();

} // namespace Sources
