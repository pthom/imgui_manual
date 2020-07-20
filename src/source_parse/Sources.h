#pragma once
#include <vector>
#include <string>
#include <map>
#include <ostream>


namespace SourceParse
{
using SourceCode = std::string;
using SourcePath = std::string;
using LibraryFolderPath = std::string;

struct SourceFile
{
    SourcePath sourcePath;
    SourceCode sourceCode;
};

struct LineWithTag
{
    int lineNumber;
    std::string tag; // tag can be an Id or a title
    int level = 0; // optional title level
};
using LinesWithTags = std::vector<LineWithTag>;

using NumberedLine = std::pair<size_t, std::string>;
using NumberedLines = std::vector<NumberedLine>;

struct AnnotatedSource
{
    SourceFile source;
    LinesWithTags linesWithTags;
};

struct Library
{
    LibraryFolderPath path;
    std::string name;
    std::string url;
    std::string shortDoc; // markdown
    std::vector<SourcePath> sourcePaths;
};

std::vector<Library> imguiLibrary();
std::vector<Library> helloImGuiLibrary();
std::vector<Library> imguiManualLibrary();
std::vector<Library> otherLibraries();
std::vector<Library> acknowldegmentLibraries();


SourceFile ReadSource(const std::string sourcePath);

inline std::ostream& operator<<(std::ostream& os, const SourceParse::LineWithTag& t)
{
    os << "{line:" << t.lineNumber << ", " << t.tag << ", level:" << t.level << "}";
    return os;
}

} // namespace SourceParse
