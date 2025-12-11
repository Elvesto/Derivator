#include <stdio.h>

#include "tree.h"
#include "tools.h"
#include "graph.h"
#include "differen.h"
#include "tex_Dump.h"

int main(const int argc, const char* const argv[]) {
    FileParam fileLoadTree = {}, texDump = {};
    if (CreateFileParam(argc, argv, &fileLoadTree) == -1) {
        return 0;
    }

    Tree tree = {};
    Tree dif = {};
    const char* dumpFileName = "dumpFile.html";
    const char* dumpFileName2 = "dumpFile2.html";
    
    texDump.fileName = "tex/texDump.tex";
    texDump.file = fopen(texDump.fileName, "w");

    LoadTree(&tree, &fileLoadTree, dumpFileName);
    // TreeInit(&tree, OP, dumpFileName);
    TreeInit(&dif, OP, dumpFileName2);
    Node* d = Derivate(tree.root);
    free(dif.root);
    dif.root = d;

    TreeDump(&dif);
    TreeDump(&tree);

    TexDump(&dif, texDump);

    TreeDestroy(&tree);
    TreeDestroy(&dif);
    
    fclose(fileLoadTree.file);

    return 0;
}