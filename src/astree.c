#include "astree.h"

AST *astreeCreate(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3){
    AST *newNode = 0;
    newNode = (AST*) calloc(1, sizeof(AST));
    newNode->symbol = symbol;
    newNode->type = type;
    newNode->son[0] = s0;
    newNode->son[1] = s1;
    newNode->son[2] = s2;
    newNode->son[3] = s3;
    return newNode;
}

void astreePrint(AST *node, int level){
    if(!node) return;

    for(int i = 0; i < level; i++){
        fprintf(stderr, "  ");
    }
    
    fprintf(stderr, "AST: (");
    
    switch(node->type){
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL, "); break;
        case AST_ADD: fprintf(stderr, "AST_ADD"); break;
        case AST_MUL: fprintf(stderr, "AST_MUL"); break;
        default: break;
    }

    if(node->symbol != NULL){
        fprintf(stderr, "%s)\n", node->symbol->text);
    } else {
        fprintf(stderr, ")\n");
    }

    for(int i = 0; i < MAX_SONS; i++){
        astreePrint(node->son[i], level+1);
    }
}