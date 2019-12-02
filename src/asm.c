#import "asm.h"

int l = 0;

int booleanToInt(char *boolean){
    if(strncmp(boolean, "TRUE", 4))
        return 1;
    return 0;
}

void generateASMVariables(AST* node, FILE* fout){
    if(!node) return;
    switch(node->type){
        case AST_PARAM:
            fprintf(stderr, "\nparam type: %d\n", node->son[0]->type);
            
        case AST_VARDEC:
            fprintf(fout, "_%s:\n"
                        "\t.globl	_%s\n"
                        "\t.type  _%s, @object\n"
                        , node->symbol->text, node->symbol->text, node->symbol->text);
            if(node->son[0]->type == AST_INT || node->son[0]->type == AST_FLOAT){
                fprintf(fout,
                        "\t.long	%s\n"
                        "\t.align   4\n"
                        "\t.size	_%s, 4\n", 
                        node->type == AST_VARDEC ? node->son[1]->symbol->text : "0",
                        node->symbol->text);
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
                int asciiForZero = 48;
                fprintf(fout,
                        "\t.byte	%d\n"
                        "\t.size	_%s, 1\n", 
                        node->type == AST_VARDEC ? node->son[1]->symbol->text[1] : asciiForZero, 
                        node->symbol->text);
            }
            if(node->son[0]->type == AST_LONG){
                fprintf(fout,
                        "\t.quad	%s\n"
                        "\t.align   8\n"
                        "\t.size	_%s, 8\n", 
                        node->type == AST_VARDEC ? node->son[1]->symbol->text : "0",
                        node->symbol->text);
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
        
        case TAC_FUNCCALL:
            fprintf(fout, "\n##TAC_FUNCCALL\n"
                            "\tmovl	$0, %%eax\n"
                            "\tcall	%s\n",
                            tac->op1->text);
        
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