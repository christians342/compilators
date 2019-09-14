#include "astree.h"

AST *astreeCreate(int type, AST *s0, AST *s1, AST *s2, AST *s3){
    AST *newNode = 0;
    newNode = (AST*) calloc(1, sizeof(AST));
    newNode->type = type;
    newNode->son[0] = s0;
    newNode->son[0] = s1;
    newNode->son[0] = s2;
    newNode->son[0] = s3;
    return newNode;
}

void astreePrint(AST *node, int level){
    if(!node) return;
    switch(node->type){
        case AST_SYMBOL:
            fprintf(stderr, "AST_SYMBOL");
            break;
        default:
            break;
        for(int i = 0; i < MAX_SONS; i++){
            astreePrint(node->son[i], level+1);
        }
    }
}