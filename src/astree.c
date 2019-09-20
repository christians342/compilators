#include "astree.h"

AST *astreeCreate(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3){
    AST *newNode = 0;
    newNode = (AST*) calloc(1, sizeof(AST));
    newNode->symbol = symbol;
    newNode->type = type;
    newNode->son[0] = s0;
    newNode->son[0] = s1;
    newNode->son[0] = s2;
    newNode->son[0] = s3;
    return newNode;
}

void astreePrint(AST *node, int level){
    if(node == NULL) return;

    for(int i = 0; i < level; ++i){
        fprintf(stderr, "  ");
    }
    
    fprintf(stderr, "AST: ");
    
    switch(node->type){
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL, \n"); break;
        case AST_ADD: fprintf(stderr, "AST_ADD, \n"); break;
        case AST_MUL: fprintf(stderr, "AST_MUL, \n"); break;
        default: break;
    }

    if(node->symbol != NULL){
        fprintf(stderr, "%s", node->symbol->text);
    } else {
        fprintf(stderr, "  ");
    }

    for(int i = 0; i < MAX_SONS; i++){
        astreePrint(node->son[i], level+1);
    }
}