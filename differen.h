#ifndef DIFFEREN_H_
#define DIFFEREN_H_

#include "tree.h"

#define ADD_(a, b) NewNodePro(OP, {.typeOp = ADD}, a, b)
#define MUL_(a, b) NewNodePro(OP, {.typeOp = MUL}, a, b)

Node* CopyTree(Node* node);

Node* Derivate(Node* node);

#endif // DIFFEREN_H_