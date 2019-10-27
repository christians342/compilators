#include "semantic.h"

int semanticErrors = 0;
int getSemanticErrors(){
    return semanticErrors;
}

int isNumeric(AST* node) {
    if(node->type == AST_INT || node->type == AST_FLOAT || node->type == AST_BYTE) 
        return 1;
    return 0;
}

int areNumericEquivalents(AST* node1, AST* node2) {

    if(isNumeric(node1) && isNumeric(node2))
        return 1;
    return 0;
}

int isVectorType(AST* node){

    if(node->type == AST_VEC || AST_VECREAD || AST_VECEXP)
        return 1;

    return 0;
}

void setTypes(AST *node){

    if(node->type == AST_VARDEC)
        node->symbol->type = SYMBOL_SCALAR;

    if(node->type == AST_FUNC)
        node->symbol->type = SYMBOL_FUNC; 

    if(isVectorType(node))
        node->symbol->type = SYMBOL_VECTOR;
}

void setDataTypes(AST *node){

     if(node->son[0]->type == AST_INT)
        node->symbol->datatype = DATATYPE_INT;

     if(node->son[0]->type == AST_FLOAT)
        node->symbol->datatype = DATATYPE_FLOAT;

     if(node->son[0]->type == AST_BYTE)
        node->symbol->datatype = DATATYPE_BYTE;

     if(node->son[0]->type == AST_BOOL)
        node->symbol->datatype = DATATYPE_BOOL;
}

void checkAndSetTypes(AST *node){
    if(!node) return;

    if(node->type == AST_VARDEC || node->type == AST_FUNC || isVectorType(node)){

        if(node->symbol){
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Symbol %s already declared at line %d.\n", node->symbol->text, node->line + 1);
                semanticErrors++;
            }
            
            setTypes(node);
            setDataTypes(node);
        }
    }

    //TODO lista de parâmetros funções

    for(int i = 0; i < MAX_SONS; i++) {
        checkAndSetTypes(node->son[i]);
    }
}

void checkUndeclared() {
    semanticErrors += hashCheckUndeclared();
}


void checkOperands(AST *node) {
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
                    )){

                } else {
                    fprintf(stderr, "Semantic ERROR: Operands not compatible.\n");
                    semanticErrors++;
                }
            }
            break;

        case AST_LESSER:
        case AST_GREATER:
        case AST_LE:
        case AST_GE:

            node->type = DATATYPE_BOOL;
            if(!isNumeric(node->son[0]) || !isNumeric(node->son[1])) {
                fprintf(stderr,"Semantic ERROR in line %d. Operators must be int, byte or float. \n", node->line + 1);
                semanticErrors++;
                node->type = DATATYPE_ERROR;
            }            
            break;  

        case AST_EQUAL:
        case AST_DIF:
           node->type = DATATYPE_BOOL;
           if ((node->son[0]->type == DATATYPE_BOOL && node->son[1]->type != DATATYPE_BOOL) ||
               (node->son[1]->type == DATATYPE_BOOL && node->son[0]->type != DATATYPE_BOOL) ||
               (!areNumericEquivalents(node->son[0], node->son[1]))){

               fprintf(stderr, "Semantic ERROR in line %d. Operators are not compatible. \n", node->line + 1);
               semanticErrors++;
               node->type = DATATYPE_ERROR;
            }

            break;

        case AST_NOT:
            node->type = DATATYPE_BOOL;
            if(node->son[0]->type != DATATYPE_BOOL){
                fprintf(stderr,"Semantic ERROR in line %d. Operator must be bool.\n", node->line + 1);
                semanticErrors++;
                node->type = DATATYPE_ERROR;
            }

            break;

        case AST_AND:
        case AST_OR:
            node->type = DATATYPE_BOOL;
            if(node->son[0]->type != DATATYPE_BOOL || node->son[1]->type != DATATYPE_BOOL){   
                fprintf(stderr,"Semantic ERROR in line %d. Operators must be bool.\n", node->line + 1);
                semanticErrors++;
                node->type = DATATYPE_ERROR;
            }
        
            break;
    }

    for(int i = 0; i < MAX_SONS; i++){
        checkOperands(node->son[i]);
    }
}

int checkSemantics(AST *node) {
    fprintf(stderr, "Checking semantics\n");
    checkAndSetTypes(node);
    checkUndeclared();    
    checkOperands(node);
    //checkDataTypes(node);
    return 1;
}
