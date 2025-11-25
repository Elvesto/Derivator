#include <stdio.h>

#include "tree.h"
#include "tools.h"
#include "graph.h"
#include "differen.h"

int main(const int argc, const char* const argv[]) {
    FileParam fileLoadTree = {};
    if (CreateFileParam(argc, argv, &fileLoadTree) == -1) {
        return 0;
    }

    Tree tree = {};
    Tree dif = {};
    const char* dumpFileName = "dumpFile.html";
    const char* dumpFileName2 = "dumpFile2.html";
    LoadTree(&tree, &fileLoadTree, dumpFileName);
    // TreeInit(&tree, OP, dumpFileName);
    TreeInit(&dif, OP, dumpFileName2);
    Node* d = Derivate(tree.root);
    dif.root = d;

    TreeDump(&tree);
    TreeDump(&dif);

    TreeDestroy(&tree);
    TreeDestroy(&dif);
    
    fclose(fileLoadTree.file);

    return 0;
}