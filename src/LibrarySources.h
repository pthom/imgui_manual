#pragma once
#include <vector>
#include <string>
#include <map>

struct LibrarySources
{
    std::string path;
    std::string name;
    std::string url;
    std::vector<std::string> sources;
};

std::vector<LibrarySources> imguiSources();
std::vector<LibrarySources> thisDemoSources();
std::vector<LibrarySources> otherLibrariesSources();

using LinesWithNotes = std::map<int, std::string>;

struct Source
{
    std::string sourcePath;
    std::string sourceCode;
};

struct AnnotatedSourceCode
{
    Source source;
    LinesWithNotes linesWithNotes;
};


Source ReadSource(const std::string sourcePath);
AnnotatedSourceCode ReadAnnotatedSource(const std::string sourcePath);
