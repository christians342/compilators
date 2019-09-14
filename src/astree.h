#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#define AST_SYMBOL 1
#define AST_ADD 2

#define MAX_SONS 4

typedef struct astree_node{
    int type;
    HASH_NODE *symbol;
    struct astree_node *son[MAX_SONS];
} AST;

AST *astreeCreate(int type, AST *s0, AST *s1, AST *s2, AST *s3);
void astreePrint(AST *node, int level);

#endif