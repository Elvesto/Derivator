#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdio.h>

#include "tree.h"

enum GraphError {
    GOOD,
    BAD,
};

static const unsigned SIZE_STR_COMPILE_COMMAND = 128;
// FIXME - const
int GraphCreate(const Tree* const tree, const char* dotName, int graphNumber);

TreeError TreeDump(Tree* tree);

#endif // GRAPH_H_