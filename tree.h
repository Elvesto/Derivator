#ifndef THREE_H_
#define THREE_H_

#include <stdio.h>
#include <stdint.h>

#include "tools.h"

enum TypeNode {
    OP = 1,
    VAR = 2,
    NUM = 3,
};

enum TypeOperation {
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    POW = 5,
    LN  = 6, 
    SIN = 7,
    COS = 8,
    TG  = 9,
    CTG = 10
};

enum TypeVar {
    X = 'x',
    Y = 'y',
    Z = 'z'
};

union ValueType {
    double typeNum;
    TypeOperation typeOp;
    TypeVar typeVar;
};

typedef TypeNode NodeType;

const int COUNT_START_ELEMENT = 1;
const int START_SIZE_ARRAY = 32;

enum TreeError {
    OK,
    ALLOCATION_ERROR,
    NOT_OK
};

struct Node {
    Node* parent;
    NodeType data;
    ValueType value;
    Node* left;
    Node* right;
};
// FIXME Field with FILE* dump_file
// typedef Node Tree;
struct Tree {
    Node* root;
    FileParam dumpFile;
    uint64_t size;
};

TreeError TreeInit(Tree* tree, NodeType value, const char* dumpFileName);

TreeError TreeDestroy(Tree* tree);

TreeError UploadTree(Tree* tree, FILE* stream);

//TreeError NodeInsert(Node* root, NodeType value);

TreeError NodesDestroy(Node* node);

Node* NewNode(NodeType value);

TreeError LoadTree(Tree* tree, FileParam* fileInfo, const char* dumpFileName);

int Definition(Node* node);

Node* Search(Node* node, TypeNode target);

void Compare(Node* node1, Node* node2);

void ParseType(Node* node, const char* idx);

Node* NewNodePro(NodeType NodeType, ValueType value, Node* left, Node* right);

#endif // THREE_H_