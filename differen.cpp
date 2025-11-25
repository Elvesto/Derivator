#include "differen.h"

#include <stdio.h>
#include <assert.h>

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
                    temp = ADD_(Derivate(node->left), Derivate(node->right));;
                    break;
                }
                case SUB: {
                    temp = ADD_(Derivate(node->left), Derivate(node->right));;
                    temp->value.typeOp = SUB;
                    break;
                }
                case MUL: {
                    Node* cR = CopyTree(node->right);
                    Node* cL = CopyTree(node->left);
                    
                    Node* dR = Derivate(node->right);
                    Node* dL = Derivate(node->left);

                    temp = ADD_(MUL_(dL, cR), MUL_(cL, dR));
                    break;
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