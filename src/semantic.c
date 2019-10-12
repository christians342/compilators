#include "semantic.h"

int semanticErrors = 0;
int getSemanticErrors(){
    return semanticErrors;
}

void checkAndSetTypes(AST *node){
    if(!node) return;

    if(node->type == AST_VARDEC || node->type == AST_FUNC){
        if(node->symbol){
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Symbol %s already declared.\n", node->symbol->text);
                semanticErrors++;
            }
            
            if(node->type == AST_VARDEC)
                node->symbol->type = SYMBOL_SCALAR;
            if(node->type == AST_FUNC)
                node->symbol->type = SYMBOL_FUNC;

            if(node->son[0]->type == AST_INT)
                node->symbol->datatype = DATATYPE_INT;
            if(node->son[0]->type == AST_FLOAT)
                node->symbol->datatype = DATATYPE_FLOAT;
        }
    }

    for(int i = 0; i < MAX_SONS; i++){
        checkAndSetTypes(node->son[i]);
    }
}

void checkUndeclared(){
    semanticErrors += hashCheckUndeclared();
}

void checkOperands(AST *node){
    if(!node) return;

    switch(node->type){
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV:
            // CHECK CORRECTNESS OF TWO OPERANDS
            for(int operand = 0; operand < 2; operand++){
                if(node->son[operand]->type == AST_ADD || 
                    node->son[operand]->type == AST_SUB || 
                    node->son[operand]->type == AST_MUL || 
                    node->son[operand]->type == AST_DIV ||
                    (
                     node->son[operand]->type == AST_SYMBOL &&
                     node->son[operand]->symbol->type == SYMBOL_SCALAR &&
                     node->son[operand]->symbol->datatype != DATATYPE_BOOL
                    ) ||
                    (
                     node->son[operand]->type == AST_SYMBOL &&
                     node->son[operand]->symbol->type == SYMBOL_LITINT
                    )||
                    (
                     node->son[operand]->type == AST_SYMBOL &&
                     node->son[operand]->symbol->type == SYMBOL_LITINT
                    )){

                } else {
                    fprintf(stderr, "Semantic ERROR: Operands not compatible.\n");
                    semanticErrors++;
                }
            }
            break;
    }

    for(int i = 0; i < MAX_SONS; i++){
        checkOperands(node->son[i]);
    }
}