#include "semantic.h"


int semanticErrors = 0;
int getSemanticErrors(){
    return semanticErrors;
}

int isBool(AST* node) {
    if(node->datatype == DATATYPE_BOOL)
        return 1;
    return 0;
}

int isNumeric(AST* node) {
    if(node->type == AST_INT || node->type == AST_FLOAT || node->type == AST_BYTE) 
        return 1;
    return 0;
}

int isNumericDatatype(int datatype) {
     if(datatype == DATATYPE_INT || datatype == DATATYPE_FLOAT 
        || datatype == DATATYPE_BYTE || datatype == DATATYPE_CHAR) 
        return 1;
    return 0;
}

int areNumericEquivalents(AST* node1, AST* node2) {

    if(isNumeric(node1) && isNumeric(node2))
        return 1;
    return 0;
}

int checkArithmeticDatatype(AST* node1, AST* node2){
    int datatype = DATATYPE_BYTE;

    if(node1->datatype == DATATYPE_BOOL || node2->datatype == DATATYPE_BOOL ||
        node1->datatype == DATATYPE_ERROR || node2->datatype == DATATYPE_ERROR)
        return DATATYPE_ERROR;

    if(node1->datatype == DATATYPE_INT || node2->datatype == DATATYPE_INT)
        datatype = DATATYPE_INT;

    if(node1->datatype == DATATYPE_LONG || node2->datatype == DATATYPE_LONG)
        datatype = DATATYPE_LONG;

    if(node1->datatype == DATATYPE_FLOAT || node2->datatype == DATATYPE_FLOAT)
        datatype = DATATYPE_FLOAT;

    return datatype;
}

int areDatatypeEquivalents(int datatype1, int datatype2) {
    if(isNumericDatatype(datatype1) && isNumericDatatype(datatype2)) 
        return 1; 
    return 0;
}


int checkVet(AST* node, int datatype)
{   
    if(node){

        if((node->son[0]->symbol->datatype != datatype) && !areDatatypeEquivalents(node->son[0]->symbol->datatype, datatype)) 
            return 0;
        if(node->son[1])
            return checkVet(node->son[1], datatype);
    }
    return 1;
}

int isVectorType(AST* node){

    if(node->type == AST_VEC || node->type == AST_VECREAD || node->type == AST_VECEXP)
        return 1;

    return 0;
}

void isReturnCompatible(AST* node, int datatype){
    if(!node) return;
    if(node->type == AST_RET){
        if(node->son[0]->datatype != datatype
            && (!isNumericDatatype(node->son[0]->datatype) || !isNumericDatatype(datatype))){
            printf("Semantic ERROR in line %d: Return statement with wrong datatype.\n", node->line + 1);
            semanticErrors++;
        }
    }
    for(int i = 0; i < MAX_SONS; i++){
        isReturnCompatible(node->son[i], datatype);
    }
}

void checkReturns(AST* node){
    if(node != NULL && node->type == AST_FUNC){
        isReturnCompatible(node, node->symbol->datatype);
    }

    for(int i = 0; i < MAX_SONS; i++){
        if(node->son[i] != NULL)
            checkReturns(node->son[i]);
    }
}

void setSymbolTypes(AST *node){
    if(node->type == AST_VARDEC)
        node->symbol->type = SYMBOL_SCALAR;

    if(node->type == AST_FUNC)
        node->symbol->type = SYMBOL_FUNC; 

    if(isVectorType(node))
        node->symbol->type = SYMBOL_VECTOR;
}

void setDataTypes(AST *node){

    if(node->son[0]->type == AST_LONG)
        node->symbol->datatype = DATATYPE_LONG;

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

    if(node->type == AST_VARDEC || node->type == AST_FUNC || node->type == AST_VEC){

        if(node->symbol){
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Symbol %s already declared at line %d.\n", node->symbol->text, node->line + 1);
                semanticErrors++;
            }
            
            setSymbolTypes(node);
            setDataTypes(node);
            node->datatype = node->symbol->datatype;
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
            node->datatype = checkArithmeticDatatype(node->son[0], node->son[1]);
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
                    node->datatype = DATATYPE_ERROR;
                }
            }
            break;

        case AST_LESSER:
        case AST_GREATER:
        case AST_LE:
        case AST_GE:
            node->datatype = DATATYPE_BOOL;
            if(!isNumeric(node->son[0]) || !isNumeric(node->son[1])) {
                fprintf(stderr,"Semantic ERROR in line %d. Logic operation. Operands must be int, byte, float or long. \n", node->line + 1);
                semanticErrors++;
                node->datatype = DATATYPE_ERROR;
            }
            node->datatype = DATATYPE_BOOL;            
            break;  

        case AST_EQUAL:
        case AST_DIF:
           node->datatype = DATATYPE_BOOL;
           if ((isBool(node->son[0]) && !isBool(node->son[1])) || (isBool(node->son[1]) && !isBool(node->son[0])) ||
               (!areNumericEquivalents(node->son[0], node->son[1]))){

               fprintf(stderr, "Semantic ERROR in line %d. Logic operation. Operands are not compatible. \n", node->line + 1);
               semanticErrors++;
               node->datatype = DATATYPE_ERROR;
            }

            break;

        case AST_NOT:
            node->datatype = DATATYPE_BOOL;
            if(!isBool(node->son[0])){
                fprintf(stderr,"Semantic ERROR in line %d. Logic operation. Operand must be bool.\n", node->line + 1);
                semanticErrors++;
                node->datatype = DATATYPE_ERROR;
            }

            break;

        case AST_AND:
        case AST_OR:
            node->datatype = DATATYPE_BOOL;
            if(!isBool(node->son[0]) || !isBool(node->son[1])){   
                fprintf(stderr,"Semantic ERROR in line %d. Logic operation. Operands must be bool.\n", node->line + 1);
                semanticErrors++;
                node->datatype = DATATYPE_ERROR;
            }
        
            break;

        case AST_WHILE:
        case AST_IF:
        case AST_IFELSE:
            node->datatype = DATATYPE_BOOL;

            if(!isBool(node->son[0])){
                fprintf(stderr,"Semantic ERROR in line %d. Conditional operation. Operand must be bool.\n", node->line + 1);
                semanticErrors++;
                node->datatype = DATATYPE_ERROR;

            }
            break;

        case AST_PRINT:
            if(node->son[0]->symbol->type != SYMBOL_LITSTRING){
                fprintf(stderr,"Semantic ERROR in line %d. Print command expected string.\n", node->line + 1);
                semanticErrors++;
                node->datatype = DATATYPE_ERROR;
            }
            break;

        case AST_READ:
            if(node->symbol->type != SYMBOL_SCALAR){
                fprintf(stderr,"Semantic ERROR in line %d. Read command expected scalar.\n", node->line + 1);
                semanticErrors++;
                node->type = DATATYPE_ERROR;
            }
            break;

        case AST_VEC:
            if(!checkVet(node->son[2], node->symbol->datatype)){
                fprintf(stderr,"Semantic ERROR in line %d. Invalid types in vector declaration\n", node->line + 1);
                semanticErrors++;
            }
            break;

        case AST_VECEXP:
        case AST_VECREAD:        
            if(!isNumericDatatype(node->son[0]->symbol->datatype)){
                fprintf(stderr,"Semantic ERROR in line %d. Invalid type in vector index\n", node->line + 1);
                semanticErrors++;
            }
            break;

        case AST_SYMBOL:
            node->datatype = node->symbol->datatype;
            break;

        case AST_FOR:
            node->datatype = DATATYPE_INT;
            if(!isNumericDatatype(node->son[0]->symbol->datatype) 
                || !isNumericDatatype(node->son[1]->symbol->datatype) 
                || !isNumericDatatype(node->son[2]->symbol->datatype)){

                fprintf(stderr,"Semantic ERROR in line %d. Invalid operand in for command \n", node->line + 1);
                semanticErrors++;
            }
            break;

    }

    for(int i = 0; i < MAX_SONS; i++){
        checkOperands(node->son[i]);
    }
}

void checkSemantics(AST *node) {
    fprintf(stderr, "---Checking semantics---\n");
    checkAndSetTypes(node);
    checkUndeclared();    
    checkOperands(node);
    checkReturns(node);
}
