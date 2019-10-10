#include "semantic.h"

int semanticErrors = 0;
int getSemanticErrors(){
    return semanticErrors;
}

void checkAndSetTypes(AST *node){
    if(!node) return;

    if(node->type == AST_LDECL || node->type == AST_FUNC){
        if(node->symbol){
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Symbol %s already declared.\n", node->symbol->text);
                semanticErrors++;
            }
            
            if(node->type == AST_LDECL)
                node->symbol->type = SYMBOL_SCALAR;
            if(node->type == AST_FUNC)
                node->symbol->type = SYMBOL_FUNC;

            if(node->son[0]->type == AST_INT)
                node->symbol->type = DATATYPE_INT;
            if(node->son[0]->type == AST_FLOAT)
                node->symbol->type = DATATYPE_FLOAT;
        }
    }

    for(int i = 0; i < MAX_SONS; i++){
        checkAndSetTypes(node->son[i]);
    }
}

void checkUndeclared(){
    semanticErrors += hashCheckUndeclared();
}