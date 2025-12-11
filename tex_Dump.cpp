#include "tex_Dump.h"

#include <assert.h>

#include "graph.h"

static const char* ValueToStr(const Node* node) {
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
                case X: return "{x}";
                case Y: return "{y}";
                case Z: return "{z}";
                default: return NULL;
            }
        }
        case NUM: {
            char* temp = (char*)calloc(START_SIZE_ARRAY, sizeof(char));
            snprintf(temp, START_SIZE_ARRAY, "%lg", node->value.typeNum);
            return temp;
        }
        default: return NULL;
    }
    return NULL;
}
static void WriteNode(Node* node, FILE* texFile);

void CreateTitle(FILE* texFile) {
    const char* const text =    "\\documentclass[12pt,a4paper]{extreport}\n"
                                "\\date{Документ сгенерирован: \\today}\n"
                                "\\begin{document}\n"
                                "\\maketitle\n"
                                "\\newpage\n";
    fprintf(texFile, "%s", text);
}

void TexDump(Tree* tree, FileParam texFile) {
    assert(tree);
    assert(texFile.file);

    CreateTitle(texFile.file);
    MathWrite(tree, texFile.file);
    fprintf(texFile.file, "\n\\end{document}\n");   
}

void MathWrite(Tree* tree, FILE* texFile) {
    fprintf(texFile, "\\[");
    WriteNode(tree->root, texFile);
    fprintf(texFile, "\\]");
}

void WriteNode(Node* node, FILE* texFile) {
    assert(node);
    if (node->left) {
        fprintf(texFile, "{");
        if (node->data == OP) {
            fprintf(texFile, "(");
            WriteNode(node->left, texFile);
            fprintf(texFile, ")");
        }
        else {
            WriteNode(node->left, texFile);
        }
        fprintf(texFile, "}");
    }
    const char* temp =  ValueToStr(node);
    fprintf(texFile, "%s", temp);
    if (node->data == NUM) {
        free((char*)temp);
    }
    if (node->right) {
        fprintf(texFile, "{");
        if (node->data == OP) {
            fprintf(texFile, "(");
            WriteNode(node->right, texFile);
            fprintf(texFile, ")");
        }
        else {
            WriteNode(node->right, texFile);
        }
        fprintf(texFile, "}");
    }
}