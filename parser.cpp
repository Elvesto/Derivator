#include "parser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static Node* GetNumber(char** idx);
static Node* GetFunc(char** idx);
static Node* GetAS(char** idx);
static Node* GetMD(char** idx);
static Node* GetP(char** idx);
static Node* GetPower(char** idx);

Node* GetG(char** idx) {
    Node* res = GetAS(idx);
    if (res == NULL) {
        return NULL;
    }
    if (**idx != '\0' || res == NULL) {
        DEBUG_LOG("Произошла ошибка - симовол - %c\n", **idx);
    }
    return res;
}

Node* GetNumber(char** idx) {
    int value = 0;
    char* s = *idx;
    char* temp = s;
    while ('0' <= *s && *s <= '9') {
        value = value * 10 + *s - '0';
        s++;
    }
    if (s == temp) {
        DEBUG_LOG("Произошла ошибка - симовол - %c\n", *s);
        return NULL;
    }

    *idx = s;

    return NUM_(value * 1.0);
}

Node* GetFunc(char** idx) {
    char* s = *idx;
    Node* node = NewNode(OP);
    if (node == NULL) {
        return NULL;
    }
    if (strncmp(s, "ln", sizeof("ln") - 1) == 0) {
        node->data = OP;
        node->value.typeOp = LN;
        s += strlen("ln");
    } else if (strncmp(s, "sin", sizeof("sin") - 1) == 0) {
        node->data = OP;
        node->value.typeOp = SIN;
        s += strlen("sin");
    } else if (strncmp(s, "cos", sizeof("cos") - 1) == 0) {
        node->data = OP;
        node->value.typeOp = COS;
        s += strlen("cos");
    } else {
        FREE(node);
        return NULL;
    }


    if (*s == '(') {
        s++;
        node->right = GetAS(&s);
        if (node->right == NULL) {
            return NULL;
        }
        if (*s == ')') {
            s++;
        } else {
            DEBUG_LOG("Произошла ошибка - симовол - %c\n", *s);
            return NULL;
        }
    } else {
        DEBUG_LOG("Произошла ошибка - симовол - %c\n", *s);
        return NULL;
    }

    *idx = s;

    return node;
}

Node* GetAS(char** idx) {
    char* s = *idx;
    Node* node = GetMD(&s);
    if (node == NULL) {
        return NULL;
    }
    while (*s == '+' || *s == '-') {
        int op = *s;
        s++;
        Node* temp = GetMD(&s);
        if (temp == NULL) {
            return NULL;
        }
        if (op == '+') {
            node = NewNodePro(OP, {.typeOp = ADD}, node, temp);
        } else {
            node = NewNodePro(OP, {.typeOp = SUB}, node, temp);
        }
    }

    *idx = s;

    return node;
}

Node* GetMD(char** idx) {
    char* s = *idx;
    Node* node = GetPower(&s);
    if (node == NULL) {
        return NULL;
    }
    while (*s == '*' || *s == '/') {
        int op = *s;
        s++;
        Node* temp = GetPower(&s);
        if (temp == NULL) {
            return NULL;
        }
        if (op == '*') {
            node = NewNodePro(OP, {.typeOp = MUL}, node, temp);
        } else {
            node = NewNodePro(OP, {.typeOp = DIV}, node, temp);
        }
    }

    *idx = s;

    return node;
}

Node* GetP(char** idx) {
    char* s = *idx;
    Node* res = NULL;
    if (*s == '(') {
        s++;
        res = GetAS(&s);
        if (res == NULL) {
            return NULL;
        }
        if (*s == ')') {
            s++;
            *idx = s;
            return res;
        } else {
            DEBUG_LOG("Произошла ошибка - симовол - %c\n", *s);
            return NULL;
        }
    } else if ((res = GetFunc(&s)) != NULL) {
        *idx = s;
        return res;
    } else if (*s == 'x') {
        res = NewNodePro(VAR, {.typeVar = X}, NULL, NULL);
        s++;
    } else {
        res = GetNumber(&s);
        if (res == NULL) {
            return NULL;
        }
    }

    *idx = s;

    return res;
}

Node* GetPower(char** idx) {
    char* s = *idx;
    Node* node = GetP(&s);
    if (node == NULL) {
        return NULL;
    }
    while (*s == '^') {
        s++;
        Node* temp = GetP(&s);
        if (temp == NULL) {
            return NULL;
        }
        node = NewNodePro(OP, {.typeOp = POW}, node, temp);
    }

    *idx = s;

    return node;
}