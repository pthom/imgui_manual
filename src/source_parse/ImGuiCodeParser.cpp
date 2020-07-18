#include "ImGuiCodeParser.h"
#include <fplus/fplus.hpp>
#include <string>

using namespace std::literals;

namespace SourceParse {

////////////////////////////////////////////////////////////////////////////////////////////////////
//    imgui.h parsing below
////////////////////////////////////////////////////////////////////////////////////////////////////

using NumberedLine = std::pair<size_t, std::string>;
using NumberedLines = std::vector<NumberedLine>;

struct HeaderLineInfo
{
    int headerLevel = 0; // 0 means "not a title"
    std::string title;
};
HeaderLineInfo NotAHeader() { return {0, ""};}

bool isEmptyLine(const NumberedLine & line)
{
    return fplus::trim_whitespace(line.second).empty();
}

bool isTopCommentLine(const NumberedLine & line)
{
    using namespace std::literals;
    return fplus::is_prefix_of("// "s, line.second);
}

bool isInnerCommentLine(const NumberedLine & line)
{
  using namespace std::literals;
  const std::string& lineCode = line.second;
  if (! fplus::is_prefix_of("  "s, lineCode))
      return false;
  return fplus::is_prefix_of(std::string("//"), fplus::trim_whitespace_left(lineCode));
}

bool isTopHeaderDashes(const NumberedLine & line)
{
    std::string headerMark = "//------";
    return fplus::is_prefix_of(headerMark, line.second);
}

bool isInnerHeaderDashes(const NumberedLine & line)
{
    using namespace std::literals;
    std::string headerMark = "//------";
    if (! fplus::is_prefix_of("  "s, line.second))
        return false;
    return fplus::is_prefix_of(headerMark, fplus::trim_whitespace_left(line.second));
}

bool isTopCommentOrHeaderDashes(const NumberedLine & line)
{
    return isTopCommentLine(line) || (isTopHeaderDashes(line));
}
bool isInnerCommentOrHeaderDashes(const NumberedLine & line)
{
    return isInnerCommentLine(line) || (isInnerHeaderDashes(line));
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

  if (!isTopCommentLine(numberedLines[idxLine]))
      return NotAHeader();
  if (idxLine < 1)
      return NotAHeader();
  if (! isTopHeaderDashes(numberedLines[idxLine - 1]))
      return NotAHeader();
  if ((idxLine > 2) && (isTopCommentLine(numberedLines[idxLine - 2])))
      return NotAHeader();
  auto remainingLines = fplus::drop(idxLine, numberedLines);
  auto followingCommentLines = fplus::take_while(isTopCommentOrHeaderDashes, remainingLines);
  auto lastCommentLine = followingCommentLines.back();
  bool isHeader = isTopHeaderDashes(lastCommentLine);
  if (!isHeader)
      return NotAHeader();

  std::string title = extractTitleFromCommentLine(line);
  return {1, title};
};


// H2 titles have 2 types:
// Case 1: Several left aligned single line comments (with an empty line before)
// followed by a struct or enum
//    -> title = struct or enum name
//    See example below
/*

// Shared state of InputText(), passed as an argument to your callback when a ImGuiInputTextFlags_Callback* flag is used.
// The callback function should return 0 by default.
struct ImGuiInputTextCallbackData
{
*/
    // Case 2: Several left aligned single line comments (with an empty line before)
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

    if (!isTopCommentLine(numberedLines[idxLine]))
        return NotAHeader();
    if (isTopHeaderDashes(numberedLines[idxLine]))
        return NotAHeader();
    if (idxLine >= 1 )
    {
        auto previousLine = numberedLines[idxLine - 1];
        if (!isEmptyLine(previousLine))
            return NotAHeader();
    }

    auto remainingLines = fplus::drop(idxLine, numberedLines);
    auto followingCommentLines = fplus::take_while(isTopCommentLine, remainingLines);

    std::string title = "";
    size_t nextSourceLineIdx = idxLine + followingCommentLines.size();
    // See if a struct or enum is defined after the comment lines
    if (nextSourceLineIdx < numberedLines.size() - 1)
    {
      std::string nextSourceLine = numberedLines[nextSourceLineIdx].second;
      std::string nextSourceLine2 = numberedLines[nextSourceLineIdx + 1].second;
      if (fplus::is_prefix_of("struct "s, nextSourceLine) && fplus::is_prefix_of("{"s, nextSourceLine2))
          title = nextSourceLine;
      if (fplus::is_prefix_of("enum "s, nextSourceLine) && fplus::is_prefix_of("{"s, nextSourceLine2))
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
    //------------------------------------------------------------------
*/
HeaderLineInfo isH3Header(const NumberedLines& numberedLines, size_t idxLine)
{
      std::string line = numberedLines[idxLine].second;

      if (!isInnerCommentLine(numberedLines[idxLine]))
          return NotAHeader();

      if (idxLine < 1)
          return NotAHeader();
      if (! isInnerHeaderDashes(numberedLines[idxLine - 1]))
          return NotAHeader();
      auto remainingLines = fplus::drop(idxLine, numberedLines);
      auto followingCommentLines = fplus::take_while(isInnerCommentLine, remainingLines);
      auto lastCommentLine = followingCommentLines.back();

      bool isHeader = isInnerHeaderDashes(lastCommentLine);
      if (!isHeader)
          return NotAHeader();

      std::string title = fplus::drop(3, fplus::trim_whitespace_left(line));
      return {3, title};
};


// H4 headers look like this (note the empty line before)
/*

    // Arguments for the different callback events
    // - To modify the text buffer in a callback, prefer using the InsertChars() / DeleteChars() function. InsertChars() will take care of calling the resize callback if necessary.
    // - If you know your edits are not going to resize the underlying buffer allocation, you may modify the contents of 'Buf[]' directly. You need to update 'BufTextLen' accordingly (0 <= BufTextLen < BufSize) and set 'BufDirty'' to true so InputText can update its internal state.
*/
HeaderLineInfo isH4Header(const NumberedLines& numberedLines, size_t idxLine)
{
    const std::string& line = numberedLines[idxLine].second;

    if (!isInnerCommentLine(numberedLines[idxLine]))
      return NotAHeader();
    if (idxLine < 1)
      return NotAHeader();
    //if (isInnerCommentLine(numberedLines[idxLine - 1]))
      //return NotAHeader();
    std::string trimmedPreviousLine = fplus::trim_whitespace(numberedLines[idxLine - 1].second);
    if (!trimmedPreviousLine.empty())
        return NotAHeader();

    bool hasHeaderMark = isInnerHeaderDashes(numberedLines[idxLine]);
    if (hasHeaderMark)
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

    // imgui.h file content becomes indexable after the first H1 title ("Header mess")
    NumberedLines indexableLines = fplus::drop_while(
        [](const auto & line) { return  ! isTopHeaderDashes(line); },
        numberedLines
        );

    auto cleanTitle = [](const HeaderLineInfo &headerLineInfo) {
        // cut title after first sentence ends (i.e after '.', '?', '!')
        std::string title = fplus::take_while(
            [](auto c) {
                return c != '.' && c != '!' && c != '?';
            },
            headerLineInfo.title);
        title = fplus::trim_whitespace(title);
        // remove after '(' except if the title is all in ()
        if (!title.empty() && title[0] != '(')
            title = fplus::take_while([](auto c) { return c != '('; }, title);

        size_t maxTitleLength = 60;
        if (title.size() > maxTitleLength)
            title = fplus::take(maxTitleLength, title) + "...";

        // title = std::to_string(headerLineInfo.headerLevel) + "-"s + title;
        return title;
    };

    LinesWithTags linesWithTags;
    for (size_t i = 0; i < indexableLines.size(); ++i)
    {
        const auto & lineWithNumber = indexableLines[i];

        auto headerLineInfo = isHeader(indexableLines, i);
        if (headerLineInfo.headerLevel > 0)
        {
            int lineNumber = (int)lineWithNumber.first;
            linesWithTags.push_back({
                lineNumber,
                cleanTitle(headerLineInfo),
                headerLineInfo.headerLevel});
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


////////////////////////////////////////////////////////////////////////////////////////////////////
//        imgui.cpp parsing below
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string lowerCaseExceptFirstLetter(const std::string &s)
{
    auto r = fplus::to_upper_case(fplus::take(1, s)) +  fplus::to_lower_case(fplus::drop(1, s));
    return r;
}



std::string upperCaseQAndA(const std::string &s)
{
    if (fplus::take(3, s) == "Q&a")
        return "Q&A"s + fplus::drop(3, s);
    else
        return s;
}

std::string lowerCaseTitle(const std::string& s)
{
    auto words = fplus::split(' ', true, s);
    words = fplus::transform(lowerCaseExceptFirstLetter, words);
    auto title = fplus::join(" "s, words);
    title = upperCaseQAndA(title);
    return title;
}


LinesWithTags findImGuiCppDoc(const std::string &sourceCode)
{
    LinesWithTags r;
    /*
      H1 titles look like this
      MISSION STATEMENT
      =================
      H2 titles look like this
      READ FIRST
     ----------
   */
    auto lines = fplus::split('\n', true, sourceCode);

    // Given two lines, we can check whether they are a header
    // and return 0 (not header) , 1 ("H1") or 2 ("H2")
    auto isHeaderLine = [](const std::pair<std::string, std::string> &linePair) {
      int headerLevel = 0;
      if (fplus::is_prefix_of("===="s, fplus::trim_whitespace(linePair.second)))
          headerLevel = 1;
      if (fplus::is_prefix_of("----"s, fplus::trim_whitespace(linePair.second)))
          headerLevel = 2;
      return headerLevel;
    };

    for (auto idx_lines : fplus::enumerate(fplus::overlapping_pairs(lines)))
    {
        int lineNumber = (int)idx_lines.first + 1;
        auto line_pair = idx_lines.second;
        int headerLevel = isHeaderLine(line_pair);
        std::string tag = fplus::trim_whitespace(line_pair.first);
        tag = lowerCaseTitle(tag);
        if (headerLevel > 0)
            r.push_back({lineNumber, tag, headerLevel});
    }
    return r;
}

AnnotatedSource ReadImGuiCppDoc()
{
    std::string sourcePath = "imgui/imgui.cpp";
    AnnotatedSource r;
    r.source = ReadSource(sourcePath);
    r.linesWithTags = findImGuiCppDoc(r.source.sourceCode);
    return r;
}



}