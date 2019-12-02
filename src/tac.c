#import "tac.h"

TAC* makeBinOperation(int type, TAC* code0, TAC* code1, int datatype);
TAC* makeIfThen(TAC* code0, TAC* code1);
TAC* makeIfThenElse(TAC* code0, TAC* code1, TAC* code2);
TAC* makeWhile(TAC* code0, TAC* code1, HASH_NODE* label, HASH_NODE* jumpLabel);
TAC* makeFunc(TAC* code0, TAC* code1, TAC* code2);
TAC* makeFor(HASH_NODE* symbol, TAC* exp1, TAC* exp2, TAC* exp3, TAC* cmd, HASH_NODE* forLabel, HASH_NODE* jumpLabel);
TAC* makeAssign(TAC* code, HASH_NODE* node);

int l = 0;

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

int booleanToInt(char *boolean){
    if(strncmp(boolean, "TRUE", 4))
        return 1;
    return 0;
}

void generateASMVariables(AST* node, FILE* fout){
    if(!node) return;
    switch(node->type){
        case AST_VARDEC:
            fprintf(fout, "_%s:\n"
                        "\t.globl	_%s\n"
                        "\t.type  _%s, @object\n"
                        , node->symbol->text, node->symbol->text, node->symbol->text);
            if(node->son[0]->type == AST_INT || node->son[0]->type == AST_FLOAT){
                fprintf(fout,
                        "\t.long	%s\n"
                        "\t.align   4\n"
                        "\t.size	_%s, 4\n", node->son[1]->symbol->text, node->symbol->text);
            }
            if(node->son[0]->type == AST_BOOL){
                fprintf(fout,
                        "\t.long	%d\n"
                        "\t.align   4\n"
                        "\t.size	_%s, 4\n",
                        booleanToInt(node->symbol->text), 
                        node->symbol->text);
            }
            if(node->son[0]->type == AST_BYTE){
                fprintf(fout,
                        "\t.byte	%d\n"
                        "\t.size	_%s, 1\n", node->son[1]->symbol->text[1], node->symbol->text);
            }
            if(node->son[0]->type == AST_LONG){
                fprintf(fout,
                        "\t.quad	%s\n"
                        "\t.align   8\n"
                        "\t.size	_%s, 8\n", node->son[1]->symbol->text, node->symbol->text);
            }
            break;
        default:
            break;
    }

    for(int i = 0; i < MAX_SONS; i++){
		generateASMVariables(node->son[i], fout);
	}
}

void generateASMTemps(FILE* fout){
    HASH_NODE** table = getTable();

    for(int i = 0; i < HASH_SIZE; i++){
        for(HASH_NODE* node = table[i]; node; node = node->next){
            if(strncmp(node->text, "__temp", 5) == 0){
                fprintf(fout, "_%s:\n"
                            "\t.globl	_%s\n"
                            "\t.type  _%s, @object\n"
                            , node->text, node->text, node->text);

                if(node->datatype == 0 || node->datatype == DATATYPE_FLOAT || node->datatype == DATATYPE_BOOL || node->datatype == DATATYPE_INT){
                    fprintf(fout,
                            "\t.long	0\n"
                            "\t.align   4\n"
                            "\t.size	_%s, 4\n", node->text);
                }
            }
        }
    }
}

void generateASMLiterals(FILE* fout){
    HASH_NODE** table = getTable();
    int LC = 0;

    fprintf(fout, "\t.section	.rodata\n"
                              ".LC_int:\n"
                              "\t.string	\"%%d\"\n");

    for(int i = 0; i < HASH_SIZE; i++){
        for(HASH_NODE* node = table[i]; node; node = node->next){
            if(node->type == SYMBOL_LITINT || node->type == SYMBOL_LITREAL){
                fprintf(fout, "_%s:\n"
                            "\t.globl	_%s\n"
                            "\t.type  _%s, @object\n"
                            "\t.long	%s\n"
                            "\t.align   4\n"
                            "\t.size	_%s, 4\n"
                            , node->text, node->text, node->text, node->text, node->text);
            } else if(node->type == SYMBOL_LITSTRING){
                fprintf(fout, "\t.section	.rodata\n"
                              ".LC%d:\n"
                              "\t.string	%s\n", LC, node->text);
                              LC++;
            } else if(node->type == SYMBOL_LITBOOL){
                fprintf(fout, "_%s:\n"
                            "\t.globl	_%s\n"
                            "\t.type  _%s, @object\n"
                            "\t.long	%d\n"
                            "\t.align   4\n"
                            "\t.size	_%s, 4\n"
                            , node->text, node->text, node->text, 
                            strncmp(node->text, "TRUE", 4) == 0? 1 : 0, node->text);
            
            }
        }
    }
}

void generateASM(TAC* tac, FILE* fout){
    if (!tac) return;

    static int LC = 0;

    if(tac->prev){
        generateASM(tac->prev, fout);
    }

    switch(tac->type){ 
        case TAC_MOVE:
            fprintf(fout, "\n##TAC_MOVE\n"

                        "\tmovl	_%s(%%rip), %%eax\n"
                        "\tmovl	%%eax, _%s(%%rip)\n"
                        "\tmovl $0, %%eax\n",
                        tac->op1->text, tac->res->text);
            break;

        case TAC_LABEL:
            fprintf(fout, "\n.%s:\n", tac->res->text);
            l++;
            break;  

       case TAC_JUMP:
              fprintf(fout,"\n##TAC_JUMP\n" 
                    "\tjmp .%s\n", tac->res->text);
              break;


        case TAC_ADD:
            fprintf(fout, "\n##TAC_ADD\n"
                        "\tmovl	_%s(%%rip), %%edx\n"
                        "\tmovl	_%s(%%rip), %%eax\n"
                        "\taddl	%%eax, %%edx\n"
                        "\tmovl	%%edx, %%eax\n"
                        "\tmovl	%%eax, _%s(%%rip)\n"
                        "\tmovl	$0, %%eax\n",
                        tac->op1->text, tac->op2->text, tac->res->text);
            break;

        case TAC_SUB:
            fprintf(fout, "\n##TAC_SUB\n"
                        "\tmovl	_%s(%%rip), %%edx\n"
                        "\tmovl	_%s(%%rip), %%eax\n"
                        "\tsubl	%%eax, %%edx\n"
                        "\tmovl	%%edx, %%eax\n"
                        "\tmovl	%%eax, _%s(%%rip)\n"
                        "\tmovl	$0, %%eax\n",
                        tac->op1->text, tac->op2->text, tac->res->text);
            break;

        case TAC_MUL:
            fprintf(fout, "\n##TAC_MUL\n"
                            "\tmovl	 _%s(%%rip), %%edx\n"
                            "\tmovl	 _%s(%%rip), %%eax\n"
                            "\timull %%edx, %%eax\n"
                            "\tmovl	 %%eax, _%s(%%rip)\n"
                            "\tmovl	 $0, %%eax\n",
                            tac->op1->text, tac->op2->text, tac->res->text);
            break;

        case TAC_DIV:
            fprintf(fout, "\n##TAC_DIV\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tmovl	_%s(%%rip), %%ecx\n"
                            "\tcltd\n"
                            "\tidivl %%ecx\n"
                            "\tmovl	%%eax, _%s(%%rip)\n"
                            "\tmovl	$0, %%eax\n",
                            tac->op1->text, tac->op2->text, tac->res->text);
            break;

        case TAC_GREATER:
            fprintf(fout, "\n##TAC_GREATER\n"

                            "\tmovl	_%s(%%rip), %%edx\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tcmpl	%%eax, %%edx\n"
                            "\tjg .L%d\n"
                            "\tmovl $0, %%eax\n"
                            "\tjmp .L%d\n"
                            ".L%d:\n"
                            "\tmovl $1, %%eax\n"
                            ".L%d:\n"
                            "\tmovl %%eax, _%s(%%rip)\n",
                            tac->op1->text, tac->op2->text, l, l+1, l, l+1, tac->res->text);  
            l = l + 2;          
            break;

        case TAC_LESSER:
            fprintf(fout, "\n##TAC_LESSER\n"
                            "\tmovl	_%s(%%rip), %%edx\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tcmpl	%%eax, %%edx\n"
                            "\tjl  .L%d\n"
                            "\tmovl $0, %%eax\n"
                            "\tjmp .L%d\n"
                            ".L%d:\n"
                            "\tmovl $1, %%eax\n"
                            ".L%d:\n"
                            "\tmovl %%eax, _%s(%%rip)\n",
                            tac->op1->text, tac->op2->text, l, l+1, l, l+1, tac->res->text);  
            l = l + 2;
            break;

        case TAC_LE:
                        fprintf(fout, "\n##TAC_LE\n"
                            "\tmovl	_%s(%%rip), %%edx\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tcmpl	%%eax, %%edx\n"
                            "\tjle  .L%d\n"
                            "\tmovl $0, %%eax\n"
                            "\tjmp .L%d\n"
                            ".L%d:\n"
                            "\tmovl $1, %%eax\n"
                            ".L%d:\n"
                            "\tmovl %%eax, _%s(%%rip)\n",
                            tac->op1->text, tac->op2->text, l, l+1, l, l+1, tac->res->text);  
            l = l + 2;
            break;

        case TAC_GE:
                       fprintf(fout, "\n##TAC_GE\n"
                            "\tmovl	_%s(%%rip), %%edx\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tcmpl	%%eax, %%edx\n"
                            "\tjge  .L%d\n"
                            "\tmovl $0, %%eax\n"
                            "\tjmp .L%d\n"
                            ".L%d:\n"
                            "\tmovl $1, %%eax\n"
                            ".L%d:\n"
                            "\tmovl %%eax, _%s(%%rip)\n",
                            tac->op1->text, tac->op2->text, l, l+1, l, l+1, tac->res->text);  
            l = l + 2;
            break;

        case TAC_EQUAL:
                       fprintf(fout, "\n##TAC_EQUAL\n"
                            "\tmovl	_%s(%%rip), %%edx\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tcmpl	%%eax, %%edx\n"
                            "\tje  .L%d\n"
                            "\tmovl $0, %%eax\n"
                            "\tjmp .L%d\n"
                            ".L%d:\n"
                            "\tmovl $1, %%eax\n"
                            ".L%d:\n"
                            "\tmovl %%eax, _%s(%%rip)\n",
                            tac->op1->text, tac->op2->text, l, l+1, l, l+1, tac->res->text); 
            l = l + 2; 
            break;

        case TAC_DIF:
                   fprintf(fout, "\n##TAC_DIF\n"
                            "\tmovl	_%s(%%rip), %%edx\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tcmpl	%%eax, %%edx\n"
                            "\tjne  .L%d\n"
                            "\tmovl $0, %%eax\n"
                            "\tjmp .L%d\n"
                            ".L%d:\n"
                            "\tmovl $1, %%eax\n"
                            ".L%d:\n"
                            "\tmovl %%eax, _%s(%%rip)\n",
                            tac->op1->text, tac->op2->text, l, l+1, l, l+1, tac->res->text);  

            l = l + 2;
            break;
        
        case TAC_AND:
            fprintf(fout, "\n##TAC_AND\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tmovl	_%s(%%rip), %%edx\n"
                            "\tandl %%eax, %%edx\n"
                            "\tjz .L%d\n"
                            "\tmovl $1, %%eax\n"
                            "\tjmp .L%d\n"
                            ".L%d:\n"
                            "\tmovl $0, %%eax\n"
                            ".L%d:\n"
                            "\tmovl %%eax, _%s(%%rip)\n", tac->op1->text, tac->op2->text, l, l+1, l, l+1, tac->res->text);
            l = l + 2;
            break;
        
        case TAC_OR:
            fprintf(fout, "\n##TAC_OR\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tmovl	_%s(%%rip), %%edx\n"
                            "\torl %%eax, %%edx\n"
                            "\tjz .L%d\n"
                            "\tmovl $1, %%eax\n"
                            "\tjmp .L%d\n"
                            ".L%d:\n"
                            "\tmovl $0, %%eax\n"
                            ".L%d:\n"
                            "\tmovl %%eax, _%s(%%rip)\n", tac->op1->text, tac->op2->text, l, l+1, l, l+1, tac->res->text);
            l = l + 2;
            break;
            

        case TAC_BEGINFUN:
            fprintf(fout, "\n##TAC_BEGINFUN\n" 
                        "\t.globl	%s\n"
                        "\t.type	%s, @function\n"
                        "%s:\n"
                        "\tpushq	%%rbp\n"
                        "\tmovq	%%rsp, %%rbp\n",
                        tac->res->text, tac->res->text, tac->res->text);
            break;
        case TAC_ENDFUN:
            fprintf(fout, "\n##TAC_ENDFUN\n" 
                        "\tpopq	%%rbp\n"
                        "\tret\n");
            break;
        case TAC_PRINT:
            fprintf(stderr, "\nprint argument datatype %d", tac->res->datatype);
            if(tac->res->datatype == DATATYPE_STRING){
                fprintf(fout, "\n##TAC_PRINT\n" 
                            "\tleaq	.LC%d(%%rip), %%rdi\n"
                            "\tmovl	$0, %%eax\n"
                            "\tcall	printf@PLT\n",
                            LC++);
            } else if(tac->res->datatype == DATATYPE_IDENTIFIER){
                fprintf(fout, "\n##TAC_PRINT\n"
                            "\tmovl	_%s(%%rip), %%eax\n"
                            "\tmovl	%%eax, %%esi\n" 
                            "\tleaq	.LC_int(%%rip), %%rdi\n"
                            "\tmovl	$0, %%eax\n"
                            "\tcall	printf@PLT\n",
                            tac->res->text);
            }
            break;

        case TAC_IFZ:
            fprintf(fout, "\n##TAC_IFZ\n"
                    "\tmovl	 _%s(%%rip), %%eax\n"
                    "\tmovl $1, %%edx\n"
                    "\tandl %%eax, %%edx\n"
                    "\tjz .%s\n",
                     tac->op1->text, tac->res->text);
    }
    return;
}

TAC* generateCode(AST *ast, HASH_NODE* loopLabel, HASH_NODE* jumpLabel){
    if(!ast) return 0;

    TAC* code[MAX_SONS]; 

    if(ast->type == AST_WHILE || ast->type == AST_FOR){
        loopLabel = makeLabel("loop");
        jumpLabel = makeLabel("jump");
    }

    // generateCode for children first
    for(int i = 0; i < MAX_SONS; i++)
        code[i] = generateCode(ast->son[i], loopLabel, jumpLabel);


    // then itself
    switch(ast->type){
        case AST_SYMBOL:
            return tacCreate(TAC_SYMBOL, ast->symbol, 0, 0);
            break; 
        case AST_ASS:
            return makeAssign(code[0], ast->symbol);
            break;
        case AST_ADD:
            return makeBinOperation(TAC_ADD, code[0], code[1], ast->datatype);
            break;
        case AST_SUB:
            return makeBinOperation(TAC_SUB, code[0], code[1], ast->datatype);
            break;
        case AST_MUL:
            return makeBinOperation(TAC_MUL, code[0], code[1], ast->datatype);
            break;
        case AST_DIV:
            return makeBinOperation(TAC_DIV, code[0], code[1], ast->datatype);
            break;
        case AST_GREATER:
            return makeBinOperation(TAC_GREATER, code[0], code[1], ast->datatype);
            break;
        case AST_LESSER:
            return makeBinOperation(TAC_LESSER, code[0], code[1], ast->datatype);
            break;
        case AST_OR:
            return makeBinOperation(TAC_OR, code[0], code[1], ast->datatype);
            break;
        case AST_AND:
            return makeBinOperation(TAC_AND, code[0], code[1], ast->datatype);
            break;
        case AST_LE:
            return makeBinOperation(TAC_LE, code[0], code[1], ast->datatype);
            break;
        case AST_GE:
            return makeBinOperation(TAC_GE, code[0], code[1], ast->datatype);
            break;
        case AST_EQUAL:
            return makeBinOperation(TAC_EQUAL, code[0], code[1], ast->datatype);
            break;
        case AST_DIF:
            return makeBinOperation(TAC_DIF, code[0], code[1], ast->datatype);
            break;
        case AST_NOT:
            return makeBinOperation(TAC_NOT, code[0], code[1], ast->datatype);
            break;
        case AST_IF:
            return makeIfThen(code[0], code[1]);
            break;
        case AST_IFELSE:
            return makeIfThenElse(code[0], code[1], code[2]);
            break;
        case AST_WHILE:
            return makeWhile(code[0], code[1], loopLabel, jumpLabel);
            break;
        case AST_READ:
            return tacCreate(TAC_READ, ast->symbol, 0, 0);
            break;
        case AST_RET: 
            return tacJoin(code[0], tacCreate(TAC_RET, code[0]?code[0]->res:0, 0, 0));
            break;
        case AST_LPRINT:
            return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT, ast->symbol, 0, 0)), code[1]);
            break;
        case AST_EPRINT: 
            return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT, code[0]?code[0]->res:0, 0, 0)), code[1]);
            break;
        case AST_VECEXP:
            return tacJoin(code[0], tacJoin(code[1], tacCreate(TAC_VECEXP, ast->symbol, code[0]?code[0]->res:0, code[1]?code[1]->res:0)));
            break;
        case AST_VECREAD:
            return tacJoin(code[0], tacCreate(TAC_VEC, makeTemp(0), ast->symbol, code[0]?code[0]->res:0));
            break;
        case AST_IDEXP:
            return tacJoin(code[0], tacCreate(TAC_FUNCCALL, makeTemp(DATATYPE_BOOL), ast->symbol, 0));
            break;
        //case AST_LPARAM: 
        //    return tacJoin(code[1], tacJoin(code[0], tacCreate(TAC_ARGPUSH, code[0]?code[0]->res:0, 0, 0)));
        //    break;
        case AST_RPARAM:
            return code[0];
        case AST_PARAM: 
            return tacJoin(tacCreate(TAC_PARAM, ast->symbol, 0, 0), code[1]);
            break;
        case AST_FUNC:
            return makeFunc(tacCreate(TAC_SYMBOL, ast->symbol, 0, 0), code[1], code[2]);
            break;
        case AST_FOR:
            return makeFor(ast->symbol, code[0], code[1], code[2], code[3], loopLabel, jumpLabel);
            break;
        case AST_BREAK:
            return tacCreate(TAC_JUMP, jumpLabel, 0, 0);
            break;
        default:
            return tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
            break;
    }
}

TAC* makeBinOperation(int type, TAC* code0, TAC* code1, int datatype){
    return tacJoin(
                    tacJoin(code0, 
                            code1), 
                    tacCreate(type, 
                              makeTemp(datatype), 
                              code0? code0->res : 0,
                              code1? code1->res : 0));
}

TAC* makeIfThen(TAC* code0, TAC* code1){
    HASH_NODE* label = makeLabel("after_if");
    TAC* tacIf = tacCreate(TAC_IFZ, label, code0? code0->res : 0, 0);
    TAC* tacLabel = tacCreate(TAC_LABEL, label, 0, 0);

    return tacJoin(tacJoin(tacJoin(code0, tacIf), code1), tacLabel);
}

TAC* makeIfThenElse(TAC* code0, TAC* code1, TAC* code2){
    HASH_NODE* label = makeLabel("else");
    TAC* tacIf = tacCreate(TAC_IFZ, label, code0? code0->res : 0, 0);
    TAC* tacLabel = tacCreate(TAC_LABEL, label, 0, 0);

    HASH_NODE* elseLabel = makeLabel("after_else");
    TAC* tacElseLabel = tacCreate(TAC_LABEL, elseLabel, 0, 0);
    TAC* tacJump = tacCreate(TAC_JUMP, elseLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacIf, code1), tacJump), tacLabel), code2), tacElseLabel);
}

TAC* makeWhile(TAC* code0, TAC* code1, HASH_NODE* whileLabel, HASH_NODE* jumpLabel){
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

TAC* makeFor(HASH_NODE* symbol, TAC* exp1, TAC* exp2, TAC* exp3, TAC* cmd, HASH_NODE* forLabel, HASH_NODE* jumpLabel) {

    TAC* tacAttr = makeAssign(exp1, symbol);

    TAC *tacLabelBeforeForCondition = tacCreate(TAC_LABEL, forLabel, 0, 0);

    TAC *tacCondition = tacCreate(TAC_LE, makeTemp(DATATYPE_BOOL), symbol, exp2 ? exp2->res : 0);

    TAC *tacIncrement = tacCreate(TAC_INCREMENT, symbol, exp3 ? exp3->res : 0, 0);

    TAC *tacLabelAfterForLoop = tacCreate(TAC_LABEL, jumpLabel, 0, 0);

    TAC *tacIfz = tacCreate(TAC_IFZ, jumpLabel, tacCondition ? tacCondition->res : 0, 0);

    TAC *tacJumpToBeginOfFor = tacCreate(TAC_JUMP, forLabel, 0, 0);

    return tacJoin(tacAttr,                                  
        tacJoin(tacLabelBeforeForCondition,  
        tacJoin(tacCondition,
        tacJoin(tacIfz,
        tacJoin(cmd,
        tacJoin(tacIncrement,
        tacJoin(tacJumpToBeginOfFor, tacLabelAfterForLoop)))))));
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

    if(tac->type == TAC_SYMBOL) return;

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
        case TAC_PARAM:     fprintf(stderr, "TAC_PARAM"); break;
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
