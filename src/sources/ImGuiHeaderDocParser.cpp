#include <fplus/fplus.hpp>
#include <string>
#include "ImGuiHeaderDocParser.h"

namespace Sources
{

using NumberedLine = std::pair<size_t, std::string>;
using NumberedLines = std::vector<NumberedLine>;

struct HeaderLineInfo
{
    int headerLevel = 0; // 0 means "not a title"
    std::string title;
};
HeaderLineInfo NotAHeader() { return {0, ""};}

bool isLeftAlignedSingleLineComment(const NumberedLine & line)
{
    using namespace std::literals;
    return fplus::is_prefix_of("//"s, line.second);
}

bool isRightAlignedSingleLineComment(const NumberedLine & line)
{
  using namespace std::literals;
  const std::string& lineCode = line.second;
  if (! fplus::is_prefix_of("  "s, lineCode))
      return false;
  return fplus::is_prefix_of(std::string("//"), fplus::trim_whitespace_left(lineCode));
}

std::string cutStringBeforeParenthesis(const std::string& s)
{
    return fplus::take_while([](char c) { return c != '('; }, s);
}

std::string extractTitleFromCommentLine(const std::string& line)
{
    using namespace std::literals;
    std::string r = line;
    r = fplus::trim_whitespace(r);
    r = fplus::replace_tokens("//"s, ""s, r);
    r = cutStringBeforeParenthesis(r);
    return r;
}


std::string headerMark = "//------";


// in imgui.h, H1 titles look like this:
/*
//-----------------------------------------------------------------------------
// ImGuiIO   <-- this is the title line
// Communicate most settings and inputs/outputs to Dear ImGui using this structure.
// Access via ImGui::GetIO(). Read 'Programmer guide' section in .cpp file for general usage.
//-----------------------------------------------------------------------------
 */
HeaderLineInfo isH1Header(const NumberedLines& numberedLines, size_t idxLine)
{
  using namespace std::literals;
  std::string line = numberedLines[idxLine].second;

  if (! isLeftAlignedSingleLineComment(numberedLines[idxLine]))
      return NotAHeader();
  if (idxLine < 1)
      return NotAHeader();
  if (! fplus::is_prefix_of(headerMark, numberedLines[idxLine - 1].second))
      return NotAHeader();
  if ((idxLine > 2) && (isLeftAlignedSingleLineComment(numberedLines[idxLine - 2])))
      return NotAHeader();
  auto remainingLines = fplus::drop(idxLine, numberedLines);
  auto followingCommentLines = fplus::take_while(isLeftAlignedSingleLineComment, remainingLines);
  auto lastCommentLine = followingCommentLines.back();

  bool isHeader = fplus::is_prefix_of(headerMark, lastCommentLine.second);
  if (!isHeader)
      return NotAHeader();

  std::string title = extractTitleFromCommentLine(line);
  return {1, title};
};


// H2 titles have 2 types:
// Case 1: Several left aligned single line comments followed by a struct or enum
//    -> title = struct or enum name
//    See example below
/*
// Shared state of InputText(), passed as an argument to your callback when a ImGuiInputTextFlags_Callback* flag is used.
// The callback function should return 0 by default.
struct ImGuiInputTextCallbackData
{
*/
    // Case 2: Several left aligned single line comments
    // -> title = first line, without parenthesis content
    //    See example below
/*
// Enums/Flags (declared as int for compatibility with old C++, to allow using as flags and to not pollute the top of this file)
// - Tip: Use your programming IDE navigation facilities on the names in the _central column_ below to find the actual flags/enum lists!
//   In Visual Studio IDE: CTRL+comma ("Edit.NavigateTo") can follow symbols in comments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
//   With Visual Assist installed: ALT+G ("VAssistX.GoToImplementation") can also follow symbols in comments.
typedef int ImGuiCol;               // -> enum ImGuiCol_             // Enum: A color identifier for
*/
HeaderLineInfo isH2Header(const NumberedLines& numberedLines, size_t idxLine)
{
    using namespace std::literals;
    std::string line = numberedLines[idxLine].second;

    if (! isLeftAlignedSingleLineComment(numberedLines[idxLine]))
        return NotAHeader();
    if (fplus::is_prefix_of(headerMark, line))
        return NotAHeader();
    if (idxLine >= 1 )
    {
        auto previousLine = numberedLines[idxLine - 1];
        if (isLeftAlignedSingleLineComment(previousLine))
            return NotAHeader();
        if (isRightAlignedSingleLineComment(previousLine))
            return NotAHeader();
    }

    auto remainingLines = fplus::drop(idxLine, numberedLines);
    auto followingCommentLines = fplus::take_while(isLeftAlignedSingleLineComment, remainingLines);

    std::string title = "";
    size_t nextSourceLineIdx = idxLine + followingCommentLines.size();
    // See if a struct or enum is defined after the comment lines
    if (nextSourceLineIdx < numberedLines.size())
    {
      std::string nextSourceLine = numberedLines[nextSourceLineIdx].second;
      if (fplus::is_prefix_of("struct "s, nextSourceLine))
          title = nextSourceLine;
      if (fplus::is_prefix_of("enum "s, nextSourceLine))
          title = nextSourceLine;
    }
    if (title.empty())
      title = extractTitleFromCommentLine(line);

    return {2, title};
};


// H3 headers look like this (note the spacing at the beginning of lines; since we
// are in a namespace or in a struct.
/*
//------------------------------------------------------------------
// Configuration (fill once)                // Default value
// Other lines...
//------------------------------------------------------------------
*/
HeaderLineInfo isH3Header(const NumberedLines& numberedLines, size_t idxLine)
{
      std::string line = numberedLines[idxLine].second;

      if (! isRightAlignedSingleLineComment(numberedLines[idxLine]))
          return NotAHeader();
      std::string headerMarker = "//-----";

      if (idxLine < 1)
          return NotAHeader();
      if (! fplus::is_prefix_of(headerMarker, fplus::trim_whitespace_left(numberedLines[idxLine - 1].second)))
          return NotAHeader();
      auto remainingLines = fplus::drop(idxLine, numberedLines);
      auto followingCommentLines = fplus::take_while(isRightAlignedSingleLineComment, remainingLines);
      auto lastCommentLine = followingCommentLines.back();

      bool isHeader = fplus::is_prefix_of(headerMarker, fplus::trim_whitespace_left(lastCommentLine.second));
      if (!isHeader)
          return NotAHeader();

      std::string title = fplus::drop(3, fplus::trim_whitespace_left(line));
      return {3, title};
};


// H4 headers look like this
/*
// Arguments for the different callback events
// - To modify the text buffer in a callback, prefer using the InsertChars() / DeleteChars() function. InsertChars() will take care of calling the resize callback if necessary.
// - If you know your edits are not going to resize the underlying buffer allocation, you may modify the contents of 'Buf[]' directly. You need to update 'BufTextLen' accordingly (0 <= BufTextLen < BufSize) and set 'BufDirty'' to true so InputText can update its internal state.
*/
HeaderLineInfo isH4Header(const NumberedLines& numberedLines, size_t idxLine)
{
    std::string line = numberedLines[idxLine].second;

    if (!isRightAlignedSingleLineComment(numberedLines[idxLine]))
      return NotAHeader();
    if (idxLine < 1)
      return NotAHeader();
    if (isRightAlignedSingleLineComment(numberedLines[idxLine - 1]))
      return NotAHeader();

    std::string title = fplus::drop(3, fplus::trim_whitespace_left(line));
    return {4, title};
};


auto isHeader = [](const NumberedLines& numberedLines, size_t idxLine)
    -> HeaderLineInfo
{
    auto r = isH1Header(numberedLines, idxLine);
    if (r.headerLevel > 0)
        return r;

     r = isH2Header(numberedLines, idxLine);
     if (r.headerLevel > 0)
         return r;

      r = isH3Header(numberedLines, idxLine);
      if (r.headerLevel > 0)
          return r;

      r = isH4Header(numberedLines, idxLine);
      return r;
};

//
// Parse imgui.h and find H1 / H2 / H3 / H4 titles
//
LinesWithTags findImGuiHeaderDoc(const std::string &sourceCode)
{
    using namespace std::literals;

    auto lines = fplus::split('\n', true, sourceCode);
    NumberedLines numberedLines = fplus::enumerate(lines);

    // imgui.h file content becomes indexable after the first H1 titled "Header mess",
    // which looks like this:
/*
//-----------------------------------------------------------------------------
// Header mess
//-----------------------------------------------------------------------------
*/
    NumberedLines indexableLines = fplus::drop_while(
        [](const auto & line) { return  ! fplus::is_prefix_of("//----"s, line.second); },
        numberedLines
        );

    LinesWithTags linesWithTags;
    for (size_t i = 0; i < indexableLines.size(); ++i)
    {
        const auto & lineWithNumber = indexableLines[i];
        auto headerLineInfo = isHeader(indexableLines, i);
        if (headerLineInfo.headerLevel > 0)
        {
            int lineNumber = (int)lineWithNumber.first;
            linesWithTags.push_back({ lineNumber, headerLineInfo.title, headerLineInfo.headerLevel});
        }
    }
    return linesWithTags;
}


AnnotatedSource ReadImGuiHeaderDoc()
{
    std::string sourcePath = "imgui/imgui.h";
    AnnotatedSource r;
    r.source = ReadSource(sourcePath);
    r.linesWithTags = findImGuiHeaderDoc(r.source.sourceCode);
    return r;
}

}