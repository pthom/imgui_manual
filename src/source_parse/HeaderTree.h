#pragma once
#include "source_parse/Tree.h"
#include "source_parse/Sources.h"

namespace SourceParse
{

    using HeaderTree = Tree<LineWithTag>;

    HeaderTree makeHeaderTree(
        const LinesWithTags& linesWithTags,
        const LineWithTag& treeTopLeaf = { -1, "TOC", -1} );

} // namespace SourceParse


