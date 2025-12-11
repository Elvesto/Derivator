#include "differen.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <malloc.h>

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
                    temp = DIV_(SUB_(MUL_(dL(node), cR(node)), MUL_(cL(node), dR(node))), POW_(cR(node), NUM_(2)));
                    break;
                }
                case POW: {
                    temp = DerivatePow(node);
                    break;
                }
                case LN: {
                    temp = MUL_(dR(node), DIV_(NUM_(1), cR(node)));
                    break;
                }
                case SIN: {
                    temp = MUL_(COS_(cR(node)), dR(node));
                    break;
                }
                case COS: {
                    temp = MUL_(NUM_(-1), MUL_(SIN_(cR(node)), dR(node)));
                    break;
                }
                case TG: {
                    temp = DIV_(dR(node), POW_(COS_(cR(node)), NUM_(2)));
                    break;
                }
                case CTG: {
                    temp = MUL_(dR(node), DIV_(NUM_(-1), POW_(SIN_(cR(node)), NUM_(2))));
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case NUM: {
            temp = NUM_(0);
            break;
        }
        case VAR: {
            temp = NUM_(1);
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
        temp = MUL_(CopyTree(node), MUL_(dR(node), NewNodePro(OP, {.typeOp = LN}, cL(node), NULL)));
        return temp;
    }

    if (Search(node->left, VAR) != NULL) {
        Node* power = SUB_(cR(node), NUM_(1));
        temp = MUL_(cR(node), MUL_(dL(node), POW_(cL(node), power)));
        return temp;
    }

    temp = NUM_(0);

    return temp;
}

Node* DerivativeN(Node* node, int n) {
    Node** arr = (Node**)calloc((size_t)n + 1, sizeof(Node*));
    arr[0] = CopyTree(node);
    for (int i = 1; i < n + 1; i++) {
        arr[i] = Derivate(arr[i-1]);
    }
    for (int i = 0; i < n; i++) {
        NodesDestroy(arr[i]);
    }
    Node* temp = arr[n];
    free(arr);
    
    return temp;
}