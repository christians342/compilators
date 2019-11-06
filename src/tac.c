#import "tac.h"

TAC* makeBinOperation(int type, TAC* code0, TAC* code1);

TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2){
    TAC* newTac;
    newTac = (TAC*) calloc(1, sizeof(TAC));
    newTac->type = type;
    newTac->res = res;
    newTac->op1 = op1;
    newTac->op2 = op2;
    newTac->prev = 0;
    newTac->next = 0;

    return newTac;
}

TAC* generateCode(AST *ast){
    if(!ast) return 0;

    TAC* code[MAX_SONS]; 

    // generateCode for children first
    for(int i = 0; i < MAX_SONS; i++)
        code[i] = generateCode(ast->son[i]);

    // then itself
    switch(ast->type){
        case AST_SYMBOL:
            return tacCreate(TAC_SYMBOL, ast->symbol, 0, 0);
            break;
        case AST_ASS:
            return tacJoin(code[0],
                           tacCreate(TAC_MOVE,
                                     ast->symbol,
                                     code[0]? code[0]->res : 0,
                                     0));
            break;
        case AST_ADD:
            return makeBinOperation(TAC_ADD, code[0], code[1]);
            break;
        default:
            return tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
            break;
    }
}

TAC* makeBinOperation(int type, TAC* code0, TAC* code1){
    return tacJoin(
                    tacJoin(code0, 
                            code1), 
                    tacCreate(type, 
                              makeTemp(), 
                              code0? code0->res : 0,
                              code1? code1->res : 0));
}

TAC* tacJoin(TAC* l1, TAC* l2){
    TAC* t;
	if(!l1) return l2;
	if(!l2) return l1;
	t = l2;
	while(t->prev){
		t = t->prev;	
	}
	t->prev = l1;
	return l2;
}

void tacPrintSingle(TAC *tac){
    if(!tac) return;

    fprintf(stderr, "TAC(");
    switch(tac->type){
        case TAC_SYMBOL: fprintf(stderr, "TAC_SYMBOL"); break;
        case TAC_ADD: fprintf(stderr, "TAC_ADD"); break;
        case TAC_SUB: fprintf(stderr, "TAC_SUB"); break;
        case TAC_MUL: fprintf(stderr, "TAC_MUL"); break;
        case TAC_DIV: fprintf(stderr, "TAC_DIV"); break;
        case TAC_MOVE: fprintf(stderr, "TAC_MOVE"); break;
        default: fprintf(stderr, "UNKNOWN"); break;
    }

    if(tac->res) fprintf(stderr, ", %s", tac->res->text);
        else fprintf(stderr, ", 0");
    if(tac->op1) fprintf(stderr, ", %s", tac->op1->text);
        else fprintf(stderr, ", 0");
    if(tac->op2) fprintf(stderr, ", %s", tac->op2->text);
        else fprintf(stderr, ", 0");

    fprintf(stderr, ");\n");
}

void tacPrintBackwards(TAC *tac){
    for(;tac; tac = tac->prev)
        tacPrintSingle(tac);
}