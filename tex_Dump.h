#ifndef TEX_DUMP_H_
#define TEX_DUMP_H_

#include "tools.h"
#include "tree.h"

void CreateTitle(FILE* texFile);

void TexDump(Tree* tree, FileParam texFile);

void MathWrite(Tree* tree, FILE* texFile);

#endif // TEX_DUMP_H_