#ifndef DIFFEREN_H_
#define DIFFEREN_H_

#include "tree.h"

#define ADD_(a, b) NewNodePro(OP, {.typeOp = ADD}, a, b)
#define SUB_(a, b) NewNodePro(OP, {.typeOp = SUB}, a, b)
#define MUL_(a, b) NewNodePro(OP, {.typeOp = MUL}, a, b)
#define DIV_(a, b) NewNodePro(OP, {.typeOp = DIV}, a, b)

#define cL(a) CopyTree(a->left)
#define cR(a) CopyTree(a->right)
#define dL(a) Derivate(a->left)
#define dR(a) Derivate(a->right)

Node* CopyTree(Node* node);

Node* Derivate(Node* node);

#endif // DIFFEREN_H_