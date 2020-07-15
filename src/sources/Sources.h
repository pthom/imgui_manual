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
    std::string tag; // tag can be an Id or a title
    int level = 0; // optional title level
};
using LinesWithTags = std::vector<LineWithTag>;

// Show a tree gui with all the tags
// return a line number if the user selected a tag, returns -1 otherwise
int guiLinesWithTags(const LinesWithTags & linesWithTags, int currentEditorLineNumber);

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
