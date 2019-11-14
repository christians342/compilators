#import "tac.h"

TAC* makeBinOperation(int type, TAC* code0, TAC* code1);
TAC* makeIfThen(TAC* code0, TAC* code1);
TAC* makeWhile(TAC* code0, TAC* code1, HASH_NODE* label);
TAC* makeFunc(TAC* code0, TAC* code1, TAC* code2);
TAC* makeFor(TAC* code0, TAC* code1, TAC* code2, TAC* code3, TAC* code4, HASH_NODE* label);
TAC* makeExp(TAC* code);
TAC* makeAssign(TAC* code, HASH_NODE* node);
TAC* makeForTo(HASH_NODE* node,  TAC* code0, TAC* code1, TAC* code2, TAC* code3);



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

TAC* generateCode(AST *ast, HASH_NODE* label){
    if(!ast) return 0;

    TAC* code[MAX_SONS]; 

    if(ast->type == AST_WHILE)
        label = makeLabel();

    // generateCode for children first
    for(int i = 0; i < MAX_SONS; i++)
        code[i] = generateCode(ast->son[i], label);


    // then itself
    switch(ast->type){
        case AST_SYMBOL:
            return tacCreate(TAC_SYMBOL, ast->symbol, 0, 0);
            break;
        case AST_ASS:
            return makeAssign(code[0], ast->symbol);
            break;
        case AST_ADD:
            return makeBinOperation(TAC_ADD, code[0], code[1]);
            break;
        case AST_SUB:
            return makeBinOperation(TAC_SUB, code[0], code[1]);
            break;
        case AST_MUL:
            return makeBinOperation(TAC_MUL, code[0], code[1]);
            break;
        case AST_DIV:
            return makeBinOperation(TAC_DIV, code[0], code[1]);
            break;
        case AST_GREATER:
            return makeBinOperation(TAC_GREATER, code[0], code[1]);
            break;
        case AST_LESSER:
            return makeBinOperation(TAC_LESSER, code[0], code[1]);
            break;
        case AST_OR:
            return makeBinOperation(TAC_OR, code[0], code[1]);
            break;
        case AST_AND:
            return makeBinOperation(TAC_AND, code[0], code[1]);
            break;
        case AST_LE:
            return makeBinOperation(TAC_LE, code[0], code[1]);
            break;
        case AST_GE:
            return makeBinOperation(TAC_GE, code[0], code[1]);
            break;
        case AST_EQUAL:
            return makeBinOperation(TAC_EQUAL, code[0], code[1]);
            break;
        case AST_DIF:
            return makeBinOperation(TAC_DIF, code[0], code[1]);
            break;
        case AST_NOT:
            return makeBinOperation(TAC_NOT, code[0], code[1]);
            break;
        case AST_IF:
        case AST_IFELSE:
            return makeIfThen(code[0], code[1]);
            break;
        case AST_WHILE:
            return makeWhile(code[0], code[1], label);
            break;
        case AST_READ:
            return tacCreate(TAC_READ, ast->symbol,0,0);
            break;
        case AST_RET: 
            return tacJoin(code[0], tacCreate(TAC_RET, code[0]?code[0]->res:0, 0, 0));
            break;
        case AST_LPRINT:
        case AST_EPRINT: 
            return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT, code[0]?code[0]->res:0, 0, 0)), code[1]);
            break;
        case AST_VECEXP:
            return tacJoin(code[0], tacJoin(code[1], tacCreate(TAC_VECEXP, ast->symbol, code[0]?code[0]->res:0, code[1]?code[1]->res:0)));
            break;
        case AST_VECREAD:
            return tacJoin(code[0], tacCreate(TAC_VEC, makeTemp(), ast->symbol, code[0]?code[0]->res:0));
            break;
        case AST_IDEXP:
            return tacJoin(code[0], tacCreate(TAC_FUNCCALL, makeTemp(), ast->symbol, 0));
            break;
        case AST_LPARAM: 
            return tacJoin(code[1], tacJoin(code[0], tacCreate(TAC_ARGPUSH, code[0]?code[0]->res:0, 0, 0)));
            break;
        case AST_RPARAM:
            return code[0];
        case AST_PARAM: 
            return tacJoin(tacCreate(TAC_PARAMPOP, ast->symbol, 0, 0), code[1]);
            break;
        case AST_FUNC:
            return makeFunc(tacCreate(TAC_SYMBOL, ast->symbol, 0, 0), code[1], code[2]);
            break;
        case AST_FOR:
            return makeForTo(ast->symbol, code[0], code[1], code[2], code[3]);
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

TAC* makeIfThen(TAC* code0, TAC* code1){
    HASH_NODE* label = makeLabel();
    TAC* tacIf = tacCreate(TAC_IFZ, label, code0? code0->res : 0, 0);
    TAC* tacLabel = tacCreate(TAC_LABEL, label, 0, 0);

    return tacJoin(tacJoin(tacJoin(code0, tacIf), code1), tacLabel);
}


TAC* makeWhile(TAC* code0, TAC* code1, HASH_NODE* whileLabel){
    HASH_NODE* jumpLabel = makeLabel();

    TAC* whileTac = tacCreate(TAC_IFZ, jumpLabel, code0? code0->res : 0, 0);
    TAC* whileTacLabel = tacCreate(TAC_LABEL, whileLabel, 0, 0);

    TAC* jumpTac = tacCreate(TAC_JUMP, whileLabel, 0, 0);
    TAC* jumpTacLabel = tacCreate(TAC_LABEL, jumpLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(whileTacLabel, code0), whileTac), code1), jumpTac), jumpTacLabel);
}

TAC* makeFunc(TAC* symbol, TAC* params, TAC* code){
    return tacJoin(
            tacJoin(
                tacJoin(
                    tacCreate(TAC_BEGINFUN, symbol->res, 0, 0), params), 
                code), 
            tacCreate(TAC_ENDFUN, symbol->res, 0, 0));
}

TAC* makeAssign(TAC* code, HASH_NODE* symbol) {
    return tacJoin(code, tacCreate(TAC_MOVE, symbol, code? code->res : 0, 0));
}

TAC* makeFor(TAC* symbol, TAC* exp1, TAC* exp2, TAC* exp3, TAC* cmd, HASH_NODE* forLabel){

    HASH_NODE* jumpLabel = makeLabel();

    TAC* forTac = tacJoin(
                    tacJoin(
                        tacJoin(
                            tacCreate(
                                TAC_IFZ, jumpLabel, symbol? symbol->res : 0, 0
                            ),
                        exp1),
                    exp2),
                  exp3);

    TAC* forTacLabel = tacCreate(TAC_LABEL, forLabel, 0, 0);

    TAC* jumpTac = tacCreate(TAC_JUMP, forLabel, 0, 0);
    TAC* jumpTacLabel = tacCreate(TAC_LABEL, jumpLabel, 0, 0);

    return tacJoin(
                tacJoin(
                    tacJoin(                        
                            tacJoin(
                                tacJoin(forTacLabel, forTac),
                            symbol), 
                    cmd), 
                jumpTac), 
            jumpTacLabel);
}

/* FOR (P = Q TO R) S */

//KW_FOR '(' TK_IDENTIFIER ':' exp ',' exp ',' exp ')' cmd
TAC* makeForTo(HASH_NODE* symbol, TAC* exp1, TAC* exp2, TAC* exp3, TAC* cmd) {
      // P = Q
    TAC* tacAttr = makeAssign(exp1, symbol);

    // ANTES DE R
    HASH_NODE *labelBeforeForCondition = makeLabel();
    TAC *tacLabelBeforeForCondition = tacCreate(TAC_LABEL, labelBeforeForCondition, 0, 0);

    // P <= R ?

    TAC *tacCondition = tacCreate(TAC_LE, makeTemp(), symbol, exp2 ? exp2->res : 0);

    /* INCREMENTO DO P */
    TAC *tacIncrement = tacCreate(TAC_INCREMENT, symbol, exp3 ? exp3->res : 0, 0);

    /* O QUE VEM DEPOIS DO S */
    HASH_NODE *labelAfterForLoop = makeLabel();
    TAC *tacLabelAfterForLoop = tacCreate(TAC_LABEL, labelAfterForLoop, 0, 0);

    /* SE P MAIOR QUE R ENTÃO VAI PRA DEPOIS DE S (O QUE ESTÁ FORA DO LOOP */
    TAC *tacIfz = tacCreate(TAC_IFZ, labelAfterForLoop, tacCondition ? tacCondition->res : 0, 0);

    /* CONTINUAÇÃO DO LOOP */
    TAC *tacJumpToBeginOfFor = tacCreate(TAC_JUMP, labelBeforeForCondition, 0, 0);

    return tacJoin(tacAttr,                                  
        tacJoin(tacLabelBeforeForCondition,  
        tacJoin(tacCondition,
        tacJoin(tacIfz,
        tacJoin(cmd,
        tacJoin(tacIncrement,
        tacJoin(tacJumpToBeginOfFor, tacLabelAfterForLoop)))))));
}

TAC* makeExp(TAC* code) {
    return tacCreate(TAC_EXP, code?code->res:0, 0, 0);

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
        case TAC_SYMBOL:    fprintf(stderr, "TAC_SYMBOL"); break;
        case TAC_ADD:       fprintf(stderr, "TAC_ADD"); break;
        case TAC_SUB:       fprintf(stderr, "TAC_SUB"); break;
        case TAC_MUL:       fprintf(stderr, "TAC_MUL"); break;
        case TAC_DIV:       fprintf(stderr, "TAC_DIV"); break;
        case TAC_MOVE:      fprintf(stderr, "TAC_MOVE"); break;
        case TAC_IFZ:       fprintf(stderr, "TAC_IFZ"); break;
        case TAC_LABEL:     fprintf(stderr, "TAC_LABEL"); break;
        case TAC_JUMP:      fprintf(stderr, "TAC_JUMP"); break;
        case TAC_READ:      fprintf(stderr, "TAC_READ"); break;
        case TAC_GREATER:   fprintf(stderr, "TAC_GREATER"); break;
        case TAC_LESSER:    fprintf(stderr, "TAC_LESSER"); break;
        case TAC_OR:        fprintf(stderr, "TAC_OR"); break;
        case TAC_AND:       fprintf(stderr, "TAC_AND"); break;
        case TAC_NOT:       fprintf(stderr, "TAC_NOT"); break;
        case TAC_LE:        fprintf(stderr, "TAC_LE"); break;
        case TAC_GE:        fprintf(stderr, "TAC_GE"); break;
        case TAC_DIF:       fprintf(stderr, "TAC_DIF"); break;
        case TAC_EQUAL:     fprintf(stderr, "TAC_EQUAL"); break;
        case TAC_RET:       fprintf(stderr, "TAC_RET"); break;
        case TAC_PRINT:     fprintf(stderr, "TAC_PRINT"); break;
        case TAC_VEC:       fprintf(stderr, "TAC_VEC"); break;
        case TAC_VECEXP:    fprintf(stderr, "TAC_VECEXP"); break;
        case TAC_BEGINFUN:  fprintf(stderr, "TAC_BEGINFUN"); break;
        case TAC_ENDFUN:    fprintf(stderr, "TAC_ENDFUN"); break;
        case TAC_FUNCCALL:  fprintf(stderr, "TAC_FUNCCALL"); break;
        case TAC_ARGPUSH:   fprintf(stderr, "TAC_ARGPUSH"); break;
        case TAC_PARAMPOP:  fprintf(stderr, "TAC_PARAMPOP"); break;
        case TAC_BREAK:     fprintf(stderr, "TAC_BREAK"); break;
        case TAC_EXP:       fprintf(stderr, "TAC_EXP"); break;
        case TAC_INCREMENT: fprintf(stderr, "TAC_INCREMENT"); break;

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
