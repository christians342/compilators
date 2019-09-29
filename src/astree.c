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
        case AST_SUB: fprintf(stderr, "AST_SUB"); break;
        case AST_DIV: fprintf(stderr, "AST_DIV"); break;
        case AST_VECREAD: fprintf(stderr, "AST_VECREAD, "); break;
        case AST_ASS: fprintf(stderr, "AST_ASS, "); break;
        case AST_LDECL: fprintf(stderr, "AST_LDECL"); break;
	case AST_BOOL: fprintf(stderr, "AST_BOOL"); break;
	case AST_BYTE: fprintf(stderr, "AST_BYTE"); break;
	case AST_INT: fprintf(stderr, "AST_INT"); break;
	case AST_LONG: fprintf(stderr, "AST_LONG"); break;
	case AST_FLOAT: fprintf(stderr, "AST_FLOAT"); break;
        case AST_EXP: fprintf(stderr, "AST_EXP"); break;	
	case AST_FUNC: fprintf(stderr, "AST_FUNC"); break;
	case AST_ATRIB: fprintf(stderr, "AST_ATRIB"); break;
	case AST_LIT: fprintf(stderr, "AST_LIT"); break;
	case AST_PARAM: fprintf(stderr, "AST_PARAM"); break;
	case AST_LPARAM: fprintf(stderr, "AST_LPARAM"); break;
	case AST_RPARAM: fprintf(stderr, "AST_RPARAM"); break;
	case AST_LPRINT: fprintf(stderr, "AST_LPRINT"); break;
	case AST_EPRINT: fprintf(stderr, "AST_EPRINT"); break;
	case AST_VECEXP: fprintf(stderr, "AST_VECEXP"); break;
	case AST_READ: fprintf(stderr, "AST_READ"); break;
	case AST_RET: fprintf(stderr, "AST_RET"); break;
	case AST_IFELSE: fprintf(stderr, "AST_IFELSE"); break;
	case AST_IF: fprintf(stderr, "AST_IF"); break;
	case AST_WHILE: fprintf(stderr, "AST_WHILE"); break;
	case AST_FOR: fprintf(stderr, "AST_FOR"); break;
	case AST_BREAK: fprintf(stderr, "AST_BREAK"); break;
	case AST_BLOCK: fprintf(stderr, "AST_BLOCK"); break;
	case AST_LCMD: fprintf(stderr, "AST_LCMD"); break;
	case AST_RCMD: fprintf(stderr, "AST_RCMD"); break;
	case AST_EPARAM: fprintf(stderr, "AST_EPARAM"); break;
	case AST_EPARAMREST: fprintf(stderr, "AST_EPARAMREST"); break;
	case AST_IDEXP: fprintf(stderr, "AST_IDEXP"); break;
	case AST_GREATER: fprintf(stderr, "AST_GREATER"); break;
	case AST_LESSER: fprintf(stderr, "AST_LESSER"); break;
	case AST_OR: fprintf(stderr, "AST_OR"); break;
	case AST_AND: fprintf(stderr, "AST_AND"); break;
	case AST_NOT: fprintf(stderr, "AST_NOT"); break;
	case AST_LE: fprintf(stderr, "AST_LE"); break;
	case AST_GE: fprintf(stderr, "AST_GE"); break;
	case AST_EQUAL: fprintf(stderr, "AST_EQUAL"); break;
	case AST_DIF: fprintf(stderr, "AST_DIF"); break;
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
