#include "differen.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <malloc.h>

static Node* DerivatePow(Node* node);
static Node* OptimizedOP(Node* node);

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
                case ADD: temp = ADD_(dL(node), dR(node)); break;

                case SUB: temp = SUB_(dL(node), dR(node)); break;

                case MUL: temp = ADD_(MUL_(dL(node), cR(node)), MUL_(cL(node), dR(node))); break;

                case DIV: temp = DIV_(SUB_(MUL_(dL(node), cR(node)), MUL_(cL(node), dR(node))), POW_(cR(node), NUM_(2))); break;

                case POW: temp = DerivatePow(node); break;

                case LN: temp = MUL_(dR(node), DIV_(NUM_(1), cR(node))); break;

                case SIN: temp = MUL_(COS_(cR(node)), dR(node)); break;
                
                case COS: temp = MUL_(NUM_(-1), MUL_(SIN_(cR(node)), dR(node))); break;
                
                case TG: temp = DIV_(dR(node), POW_(COS_(cR(node)), NUM_(2))); break;
                
                case CTG: temp = MUL_(dR(node), DIV_(NUM_(-1), POW_(SIN_(cR(node)), NUM_(2)))); break;

                default: break;
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
    if (Search(node->right, VAR) != NULL) {
        return MUL_(CopyTree(node), ADD_(MUL_(dR(node), LN_(cL(node))), DIV_(MUL_(cR(node), dL(node)), cL(node))));
    }

    if (Search(node->left, VAR) != NULL) {
        Node* power = SUB_(cR(node), NUM_(1));
        return MUL_(cR(node), MUL_(dL(node), POW_(cL(node), power)));
    }

    return NUM_(0);
}

Node* DerivativeN(Node* node, int n) {
    Node* cur = CopyTree(node);
    for (int i = 0; i < n; i++) {
        Node* temp = Derivate(cur);
        NodesDestroy(cur);
        cur = temp;
    }
    return cur;
}

Node* OptimizedTree(Node* node) {
    assert(node);

    Node* left  = node->left  ? OptimizedTree(node->left)  : NULL;
    Node* right = node->right ? OptimizedTree(node->right) : NULL;

    node->left = left;
    if (left)  left->parent = node;

    node->right = right;
    if (right) right->parent = node;

    Node* temp = OptimizedOP(node);
    return temp;
}

Node* OptimizedOP(Node* node) {
    assert(node);

    if (node->data == OP && node->left && node->right && \
        node->left->data == NUM && node->right->data == NUM) {
        
        double a = node->left->value.typeNum;
        double b = node->right->value.typeNum;
        double res = 0;
        
        switch (node->value.typeOp) {
            case ADD: res = a + b; break;
            case SUB: res = a - b; break;
            case MUL: res = a * b; break;
            case DIV: res = a / b; break;
            case POW: res = pow(a, b); break;
            case SIN: case COS: case TG: case CTG: case LN: default: return node;
        }
        NodesDestroy(node->left);
        NodesDestroy(node->right);
        node->data = NUM;
        node->left = node->right = NULL;
        node->value.typeNum = res;
        return node;
    }
    // a + 0 = a
    if (node->data == OP && node->value.typeOp == ADD) {
        if (node->left->data == NUM) {
            if (DoubleCmp(node->left->value.typeNum, 0)) {
                Node* temp = node->right;
                temp->parent = node->parent;

                NodesDestroy(node->left);
                FreeNode(node);

                return temp;
            }
        } else if (node->right->data == NUM) {
            if (DoubleCmp(node->right->value.typeNum, 0)) {
                Node* temp = node->left;
                temp->parent = node->parent;

                NodesDestroy(node->right);
                FreeNode(node);

                return temp;
            }
        }
    } 
    // a*0 = 0, a*1 = a
    if (node->data == OP && node->value.typeOp == MUL) {
        Node* temp = NULL;
        if (node->left->data == NUM) {
            if (DoubleCmp(node->left->value.typeNum, 0)) {
                NodesDestroy(node->right);
                
                temp = NUM_(0);
                temp->parent = node->parent;

                FreeNode(node);
                return temp;
            }
            if (DoubleCmp(node->left->value.typeNum, 1)) {
                temp = node->right;
                temp->parent = node->parent;

                NodesDestroy(node->left);
                FreeNode(node);

                return temp;
            }
        }
        if (node->right->data == NUM) {
            if (DoubleCmp(node->right->value.typeNum, 0)) {
                NodesDestroy(node->left);

                temp = NUM_(0);
                temp->parent = node->parent;

                FreeNode(node);
                return temp;
            }
            if (DoubleCmp(node->right->value.typeNum, 1)) {
                temp = node->left;
                temp->parent = node->parent;

                NodesDestroy(node->right);
                FreeNode(node);

                return temp;
            }
        }
    }

    return node;
}