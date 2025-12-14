#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree.h"

static const char* PATH_GRAPH = "graphs/graph.gv";
// FIXME - naming
static int CreateNode(const Node* const node, FILE* graphFile);
static const char* DataToStr(NodeType data);
static const char* ValueToStr(const Node* node);

int GraphCreate(const Tree* const tree, const char* dotName, int graphNumber) {
    assert(tree);
    assert(dotName);

    FILE* dotFile = fopen(dotName, "w");
    if (dotFile == NULL) {
        fprintf(stderr, "Graph not created\n");
        return BAD; // FIXME уёбищная обработка ошибок
    }

    fprintf(dotFile, "digraph gh {\n");
    fprintf(dotFile, "graph [rankdir=TB];\n");
    
    CreateNode(tree->root, dotFile);

    fprintf(dotFile, "\n}\n");

    fclose(dotFile);

    char dotCompileStr[SIZE_STR_COMPILE_COMMAND] = {};
    snprintf(dotCompileStr, SIZE_STR_COMPILE_COMMAND, "dot -Tsvg %s -o images/graph%d.svg", dotName, graphNumber);

    system(dotCompileStr);

    return GOOD;
}

TreeError TreeDump(Tree* tree) {
    if (tree == NULL) {
        fprintf(stderr, "NULLPTR was passed to the TreeDump\n");
        return NOT_OK;
    }

    static int counter = 1;

    fprintf(tree->dumpFile.file, "<pre>\n");

    fprintf(tree->dumpFile.file, "<h3> THREE DUMP №%d </h3>\n", counter);

    fprintf(tree->dumpFile.file, "THREE [%p]\n", tree);

    if (GraphCreate(tree, PATH_GRAPH, counter) != GOOD) {
        return NOT_OK;
    }

    fprintf(tree->dumpFile.file, "<img src=images/graph%d.svg>\n", counter);
    counter++;

    return OK;
}

// FIXME - static func
int CreateNode(const Node* const node, FILE* graphFile) {
    assert(node);
    assert(graphFile);
    static int counter = 1;
// FIXME - answer for edges
    const char* tempStr = ValueToStr(node);
    fprintf(graphFile, "\tnode%d [label=\"%s\\n value=%s \\n ", counter, DataToStr(node->data), tempStr);
    if (node->data == NUM) {
        free((char*)tempStr);
        tempStr = NULL;
    }
    counter++;
    int temp = counter;
    fprintf(graphFile, "left = %p\\n right = %p\"];\n", node->left, node->right);
    if (node->left) {
        fprintf(graphFile, "\tnode%d -> node%d [color=red];\n", temp-1, counter);
        CreateNode(node->left, graphFile);
    }
    if (node->right) {
        fprintf(graphFile, "\tnode%d -> node%d [color=red];\n", temp-1, counter);
        CreateNode(node->right, graphFile);
    }

    return GOOD;
}

const char* DataToStr(NodeType data) {
    switch (data) {
        case OP: return "OP";
        case VAR: return "VAR";
        case NUM: return "NUM";
        default: return "ERROR";
    }
}

const char* ValueToStr(const Node* node) {
    switch (node->data) {
        case OP: {
            switch (node->value.typeOp) {
                case ADD: return "+";
                case SUB: return "-";
                case MUL: return "*";
                case DIV: return "/";
                case POW: return "^";
                case LN:  return "ln";
                case COS: return "cos";
                case SIN: return "sin";
                case TG: return "tg";
                case CTG: return "ctg";
                default: return NULL;
            }
        }
        case VAR: {
            switch(node->value.typeVar) {
                case X: return "x";
                case Y: return "y";
                case Z: return "z";
                default: return NULL;
            }
        }
        case NUM: {
            char* temp = (char*)calloc(START_SIZE_ARRAY, sizeof(char));
            snprintf(temp, START_SIZE_ARRAY, "%lf", node->value.typeNum);
            return temp;
        }
        default: return NULL;
    }
    return NULL;
}