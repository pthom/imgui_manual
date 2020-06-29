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

std::vector<LibrarySources> thisLibrarySources();
std::vector<LibrarySources> otherSources();
std::vector<LibrarySources> allSources();

using LinesWithNotes = std::map<int, std::string>;

struct AnnotatedSourceCode
{
    std::string sourcePath;
    std::string sourceCode;
    LinesWithNotes linesWithNotes;
};


// (private) LinesWithNotes findCollapsingHeaderRegions(const std::string &sourceCode);
AnnotatedSourceCode ReadSelectedLibrarySource(const std::string sourcePath);
