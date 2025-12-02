#include "differen.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>

static Node* DerivatePow(Node* node);

Node* CopyTree(Node* node) {
    assert(node);

    Node* root = NewNode(node->data);
    if (root == NULL) {
        return NULL;
    }
    root->value = node->value;

    if (node->left) {
        root->left = CopyTree(node->left);
        if (root->left) {
            root->left->parent = root;
        }
    }
    if (node->right) {
        root->right = CopyTree(node->right);
        if (root->right) {
            root->right->parent = root;
        }
    }

    return root;
}

Node* Derivate(Node* node) {
    assert(node);

    Node* temp = NULL;
    switch (node->data) {
        case OP: {
            switch (node->value.typeOp) {
                case ADD: {
                    temp = ADD_(dL(node), dR(node));
                    break;
                }
                case SUB: {
                    temp = ADD_(dL(node), dR(node));
                    temp->value.typeOp = SUB;
                    break;
                }
                case MUL: {
                    temp = ADD_(MUL_(dL(node), cR(node)), MUL_(cL(node), dR(node)));
                    break;
                }
                case DIV: {
                    temp = ADD_(MUL_(dL(node), cR(node)), MUL_(cL(node), dR(node)));
                    temp->value.typeOp = SUB;
                    break;
                }
                case POW: {
                    temp = DerivatePow(node);
                    break;
                }
                case LN: {
                    temp = MUL_(dL(node), NewNodePro(OP, {.typeOp = DIV}, NewNodePro(NUM, {.typeNum = 1}, NULL, NULL), cL(node)));
                    break;
                }
                case SIN: {
                    temp = MUL_(NewNodePro(OP, {.typeOp = COS}, cL(node), NULL), dL(node));
                    break;
                }
                case COS: {
                    temp = MUL_(NewNodePro(NUM, {.typeNum = -1}, NULL, NULL), (NewNodePro(OP, {.typeOp = COS}, cL(node), NULL), dL(node)));
                }
            }
            break;
        }
        case NUM: {
            temp = NewNodePro(NUM, {.typeNum = 0}, NULL, NULL);
            break;
        }
        case VAR: {
            temp = NewNodePro(NUM, {.typeNum = 1}, NULL, NULL);
            break;
        }
        default: {
            fprintf(stderr, "Дифференцируем нечто\n");
            return NULL;
        }
    }
    return temp;
}

Node* DerivatePow(Node* node) {
    assert(node);
    Node* temp = NULL;
    if (Search(node->right, VAR) != NULL) {
        temp = MUL_(CopyTree(node), Derivate(MUL_(NewNodePro(OP, {.typeOp = LN}, cL(node), NULL), cR(node))));
        return temp;
    }

    if (Search(node->left, VAR) != NULL) {
        Node* tempNum = NewNodePro(OP, {.typeOp = SUB}, cR(node), NewNodePro(NUM, {.typeNum = 1}, NULL, NULL));
        temp = MUL_(cR(node), MUL_(Derivate(cL(node)), NewNodePro(OP, {.typeOp = POW}, cL(node), tempNum)));
        return temp;
    }

    temp = NewNodePro(NUM, {.typeNum = 0}, NULL, NULL);

    return temp;
}