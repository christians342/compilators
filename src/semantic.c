#include "semantic.h"

void checkAndSetTypes(AST *node){
    if(!node){
        return;
    }
    if(node->type == AST_LDECL){
        if(node->symbol){
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Symbol %s already declared.\n", node->symbol->text);
            }
            node->symbol->type = SYMBOL_SCALAR;
        }
    }
    if(node->type == AST_FUNC){
        if(node->symbol){
            if(node->symbol->type != SYMBOL_IDENTIFIER)
                fprintf(stderr, "Semantic ERROR: Symbol %s already declared.\n", node->symbol->text);
            node->symbol->type = SYMBOL_FUNC;
        }
    }

    for(int i = 0; i < MAX_SONS; i++){
        checkAndSetTypes(node->son[i]);
    }
}

void checkUndeclared(){
    hashCheckUndeclared();
}