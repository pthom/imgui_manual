#include <fplus/fplus.hpp>
#include "source_parse/Sources.h"

namespace SourceParse
{

LinesWithTags findImGuiDemoCodeLines(const std::string &sourceCode)
{
    LinesWithTags r;

    static std::string regionToken = "DemoCode";

    auto extractDemoCodeName = [](const std::string &codeLine) {
      // if codeLine == "DemoCode("Line Plots")) {"
      // then return
      // "Line Plots"
      auto tokens = fplus::split('"', true, codeLine);
      if (tokens.size() >= 3)
          return tokens[1];
      else
          return std::string();
    };

    auto lines = fplus::split('\n', true, sourceCode);
    for (size_t line_number = 0; line_number < lines.size(); line_number++)
    {
        const std::string& line = lines[line_number];
        if (line.find(regionToken) != std::string::npos)
            r.push_back({(int)line_number, extractDemoCodeName(line)});
    }
    return r;
}


AnnotatedSource ReadImGuiDemoCode()
{
    std::string sourcePath = "imgui/imgui_demo.cpp";
    AnnotatedSource r;
    r.source = ReadSource(sourcePath);
    r.linesWithTags = findImGuiDemoCodeLines(r.source.sourceCode);
    return r;
}


}