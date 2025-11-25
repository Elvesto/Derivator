#include "tree.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "graph.h"
#include "tools.h"

static TreeError PrintNode(Node* node, FILE* stream);
static FileParam OpenDumpFile(const char* dumpFileName);
static TreeError CloseDumpFile(FileParam dump);

TreeError TreeInit(Tree* tree, NodeType value, const char* dumpFileName) {
    assert(tree);

    tree->root = (Node*)calloc(COUNT_START_ELEMENT, sizeof(Node));
    if (tree->root == NULL) {
        fprintf(stderr, "ALLOCATION ERROR\n");
        return ALLOCATION_ERROR;
    }

    tree->dumpFile = OpenDumpFile(dumpFileName);

    tree->root->data = value;
    tree->root->left = NULL;
    tree->root->right = NULL; 
    tree->root->parent = NULL;

    return OK;
}

FileParam OpenDumpFile(const char* dumpFileName) {
    assert(dumpFileName);
    
    FileParam dump = {.fileName = dumpFileName};
    dump.file = fopen(dumpFileName, "w+");
    if (dump.file == NULL) {
        fprintf(stderr, "FILE NO OPEN - %s\n", dumpFileName);
        dump.fileName = NULL;
        return dump;
    }

    return dump;
}

TreeError TreeDestroy(Tree* tree) {
    assert(tree);

    DEBUG_LOG ("%s", "Tree destroy");

    NodesDestroy(tree->root);

    CloseDumpFile(tree->dumpFile);

    return OK;
}

static TreeError CloseDumpFile(FileParam dump) {
    if (dump.file != NULL) {
        fclose(dump.file);
        dump.fileName = NULL;
    }

    return OK;
}

TreeError UploadTree(Tree* tree, FILE* stream) {
    assert(tree);
    assert(stream);

    PrintNode(tree->root, stream);

    return OK;
}

TreeError PrintNode(Node* node, FILE* stream) {
    assert(node);
    fprintf(stream, "(");
    fprintf(stream, "\"%d\"", node->data);
    if (node->left) {
        printf("\n\t");
        PrintNode(node->left, stream);
    } else {
        fprintf(stream, "nil ");
    }
    if (node->right) {
        printf("\n\t");
        PrintNode(node->right, stream);
    } else {
        fprintf(stream, "nil ");
    }
    fprintf(stream, ")");

    return OK;
}

// NOTE Для числового дерева 
/*
TreeError NodeInsert(Node* root, NodeType value) {
    assert(root);

    if (root->data >= value) {
        if (root->left != NULL) {
            NodeInsert(root->left, value);
        } else {
            root->left = NewNode(value);
        }
    } else {
        if (root->right != NULL) {
            NodeInsert(root->right, value);
        } else {
            root->right = NewNode(value);
        }
    }

    return OK;
}
*/

TreeError NodesDestroy(Node* node) {
    assert(node);
    static int countDestroy = 0;

    // DEBUG_LOG("node = %p", node);
    if (node->left) {
        // DEBUG_LOG("\t\tnode->left = %p", node->left);
        NodesDestroy(node->left);
        node->left = NULL;
    }
    if (node->right) {
        // DEBUG_LOG("\t\t node->right = %p", node->right);
        NodesDestroy(node->right);
        node->right = NULL;
    }

    
    FREE(node);

    countDestroy++;
    // printf("%d\n", countDestroy);

    return OK;
}


Node* NewNode(NodeType value) {
    Node* node = (Node*)calloc(COUNT_START_ELEMENT, sizeof(Node)); // FIXME - NULL ( -1 ball k ocenke)
    if (node == NULL) {
        fprintf(stderr, "AllOCATION NODE DID'T HAPPEN\n");
        return NULL;
    }
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

static Node* ReadNode(char** ppos) {
    assert(ppos);

    char* pos = *ppos;
    if (pos[0] == ' ' || pos[0] == '\t' || pos[0] == '\n') {
        *ppos = &pos[1];
        return ReadNode(ppos);
    }

    if (pos[0] == '(') {
        Node* node = NewNode(OP);
        if (node == NULL) {
            fprintf(stderr, RED_BOLD_COLOR "Allocation node didn't happen" COLOR_END "\n");
            return NULL;
        }

        pos += 2;                      // skip '(' skip '"'
        char* temp = strchr(pos, '"');
        if (temp != NULL) {
            *temp = '\0';
            ParseType(node, pos);
            *temp = '"';
            pos = temp + 1;
        }
        while (isspace(*pos)) { pos++; }  // skip spaces

        node->left = ReadNode(&pos);
        if (node->left != NULL) { node->left->parent = node; }

        node->right = ReadNode(&pos);
        if (node->right != NULL) { node->right->parent = node; }

        pos++;
        *ppos = pos;
        return node;
    } else if (strncmp(pos, "nil", sizeof("nil") - 1) == 0) {
        pos += 3;

        while (isspace(*pos)) { pos++; }        // skip spaces

        *ppos = pos;
        return NULL;
    }

    fprintf(stderr, "ERROR READ\n");
    printf("%s\n", pos);
    return NULL;
}



TreeError LoadTree(Tree* tree, FileParam* fileInfo, const char* dumpFileName) {
    assert(fileInfo);

    tree->dumpFile = OpenDumpFile(dumpFileName);

    char* buffer = BufCreate(fileInfo);
    // printf("%s\n", buffer);
    char* temp = buffer;

    tree->root = ReadNode(&temp);

    FREE(buffer);

    return OK;
}

int Definition(Node* node) {
    assert(node);

    if (node->parent) {
        printf("%d, ", node->data);
        if (node->parent->right == node) {
            printf("не ");
        }
        Definition(node->parent);
    } else {
        printf("%d, ", node->data);
    }

    // printf("%s ", node->data);

    return 0;
}

Node* Search(Node* node, TypeNode target) {
    assert(node);
    assert(node->data);

    Node* temp = {};
    
    if (target == node->data) {
        return node;
    }
    if (node->left) {
        temp = Search(node->left, target);
        if (temp != NULL) { return temp; }
    }
    if (node->right) {
        temp = Search(node->right, target);
        if (temp != NULL) { return temp; }
    }

    return NULL;
}

void ParseType(Node* node, const char* idx) {
    char c = *idx;
    switch (c) {
        case '+': 
            node->data = OP;
            node->value.typeOp = ADD;
            break;
        case '-':
            node->data = OP;
            node->value.typeOp = SUB;
            break;
        case '*':
            node->data = OP;
            node->value.typeOp = MUL;
            break;
        case '/':
            node->data = OP;
            node->value.typeOp = DIV;
            break;
        case 'x':
            node->data = VAR;
            node->value.typeVar = X;
            break;
        case 'y':
            node->data = VAR;
            node->value.typeVar = Y;
            break;
        case 'z':
            node->data = VAR;
            node->value.typeVar = Z;
            break;
        default:
            double temp = strtod(idx, NULL);
            if (temp != 0) {
                node->data = NUM;
                node->value.typeNum = temp;
            } else {
                fprintf(stderr, "Unknown %s\n", idx);
            }
            break;
    }

    return;
}

Node* NewNodePro(NodeType NodeType, ValueType value, Node* left, Node* right) {
    Node* node = NewNode(NodeType);
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}