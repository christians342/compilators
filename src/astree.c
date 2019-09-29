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
        case AST_SUB: fprintf(stderr, "AST_SUB"); break;
        case AST_MUL: fprintf(stderr, "AST_MUL"); break;
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
		case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
		case AST_VEC: fprintf(stderr, "AST_VEC"); break;
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

void printSymbol(AST* node, FILE* file) {
	if(node && node->symbol){
		fprintf(file, " %s", node->symbol->text);
	}
	else {
		printf("\nCould not find symbol\n\n");
	}	
}

 
void decompilation(AST *node, FILE *file) { 
    
	if(!node)
		return;

	switch(node->type){

		case AST_SYMBOL:
			printf("AST_SYMBOL");
            printSymbol(node, file);
            break;

        case AST_ADD:
            decompilation(node->son[0], file);
            fprintf(file, " + ");
            decompilation(node->son[1], file);
            break;

        case AST_SUB:
            decompilation(node->son[0], file);
            fprintf(file, " - ");
            decompilation(node->son[1], file);
            break;

        case AST_MUL:
            decompilation(node->son[0], file);
            fprintf(file, " * ");
            decompilation(node->son[1], file);
            break;

        case AST_DIV:
            decompilation(node->son[0], file);
            fprintf(file, " / ");
            decompilation(node->son[1], file);
            break;

        case AST_GREATER:
        	decompilation(node->son[0], file);
            fprintf(file, " > ");
            decompilation(node->son[1], file);
            break;

        case AST_LESSER:
        	decompilation(node->son[0], file);
            fprintf(file, " < ");
            decompilation(node->son[1], file);
            break;

        case AST_GE:
        	decompilation(node->son[0], file);
            fprintf(file, " >= ");
            decompilation(node->son[1], file);
            break;

        case AST_LE:
        	decompilation(node->son[0], file);
            fprintf(file, " <= ");
            decompilation(node->son[1], file);
            break;

        case AST_DIF:
        	decompilation(node->son[0], file);
            fprintf(file, " != ");
            decompilation(node->son[1], file);
            break;

        case AST_EQUAL:
        	decompilation(node->son[0], file);
            fprintf(file, " == ");
            decompilation(node->son[1], file);
            break;

        case AST_OR:
        	decompilation(node->son[0], file);
            fprintf(file, " v ");
            decompilation(node->son[1], file);
            break;

        case AST_AND:
        	decompilation(node->son[0], file);
            fprintf(file, " . ");
            decompilation(node->son[1], file);
            break;

        case AST_NOT:
            fprintf(file, " ~ ");
        	decompilation(node->son[0], file);
            break;

        case AST_ASS:
        	printf("AST_ASS");
			printSymbol(node, file);
           	fprintf(file," = "); 
           	decompilation(node->son[0], file);
           	break;

        case AST_BOOL:
           	fprintf(file," bool "); 
           	break;

        case AST_BYTE:
           	fprintf(file," byte "); 
           	break;

        case AST_INT:
           	fprintf(file," int "); 
           	break;

		case AST_LONG:
           	fprintf(file," long "); 
           	break;

        case AST_FLOAT:
        	fprintf(file," float "); 
           	break;

        case AST_WHILE:
            fprintf(file,"while("); 
            decompilation(node->son[0], file);
            fprintf(file,")"); 
            decompilation(node->son[1], file);
            break;

        case AST_FOR:
        	fprintf(file, "for(");
            printf("FOR");
            printSymbol(node, file);
            decompilation(node->son[0], file);
            fprintf(file, ", ");
            decompilation(node->son[1], file);
            fprintf(file, ", ");
            decompilation(node->son[2], file);
            fprintf(file, ")");
            decompilation(node->son[3], file);
            break;

        case AST_BREAK:
            fprintf(file, "break");
            break;

         case AST_IFELSE:
            fprintf(file, "if(");
            decompilation(node->son[0], file);
            fprintf(file, ") then \n");
            decompilation(node->son[1], file);
            fprintf(file, "else\n");
            decompilation(node->son[2], file);
            break;

        case AST_IF:
            fprintf(file, "if(");
            decompilation(node->son[0], file);
            fprintf(file, ") then \n");
            decompilation(node->son[1], file);
            break;

        case AST_BLOCK:
            fprintf(file, "{\n");
            decompilation(node->son[0], file);
            fprintf(file, "}");
            break;

        case AST_LCMD:
            decompilation(node->son[0], file);
            decompilation(node->son[1], file);
            break;

        case AST_RCMD:
            fprintf(file, ";\n");
            decompilation(node->son[0], file);
            decompilation(node->son[1], file);
            break;

        case AST_EXP:
        	fprintf(file, "(");
        	decompilation(node->son[0], file);
            fprintf(file, ")");
            break;

		case AST_FUNC:
            decompilation(node->son[0], file);
            printf("AST_FUNC");
            fprintf(file, "(");
            printSymbol(node, file); //nome função
            decompilation(node->son[1], file);
            fprintf(file, ")");
            decompilation(node->son[2], file);
            fprintf(file, ";\n");
        	break;

		case AST_ATRIB:
            decompilation(node->son[0], file);
            printf("AST_ATRIB");
            printSymbol(node, file);
            fprintf(file, " = ");
            decompilation(node->son[1], file);
            fprintf(file, ";\n");
            break;

		case AST_LIT: 
			fprintf(file, " : "); 
			decompilation(node->son[0], file);
			fprintf(file, " ");	
			decompilation(node->son[1], file);
			break;

		case AST_LPARAM:
            decompilation(node->son[0], file);
            printf("LPARAM");
            printSymbol(node, file);
            decompilation(node->son[1], file);
            break;

		case AST_RPARAM:
            fprintf(file, ", ");
            decompilation(node->son[0], file);
            break;

		case AST_LDECL:
            decompilation(node->son[0], file);
            decompilation(node->son[1], file);
            break;

        case AST_RET:
            fprintf(file, "return ");
            decompilation(node->son[0], file);
            break;

        case AST_READ:
           	printf("AST_READ");
           	fprintf(file, "read ");
            printSymbol(node, file);
            break; 

		case AST_IDEXP:
            fprintf(file, "%s(", node->symbol->text);
            decompilation(node->son[0], file);
            fprintf(file, ")");
            break;

        case AST_EPARAM:
            decompilation(node->son[0], file);
            decompilation(node->son[1], file);
            break;

        case AST_EPARAMREST:
            fprintf(file, ", ");
            decompilation(node->son[0], file);
            break;

        case AST_VECREAD:
            fprintf(file, "%s[", node->symbol->text);
            decompilation(node->son[0], file);
            fprintf(file, "] = ");
            decompilation(node->son[1], file);
            break;

        case AST_PRINT:
        	fprintf(file, "print ");
        	decompilation(node->son[0], file);
        	break;

		case AST_LPRINT: 
            printf("LPRINT");
			printSymbol(node, file); 
			decompilation(node->son[0], file);			
			break;

		case AST_EPRINT: 
			decompilation(node->son[0], file);
			decompilation(node->son[1], file);				
			break;

		case AST_VECEXP: 
            decompilation(node->son[0], file);
            printf("VECEXP");
			printSymbol(node, file);
			fprintf(file, "[ ");
			decompilation(node->son[1], file);
			fprintf(file, "] = ");
			decompilation(node->son[2], file);

        case AST_VEC:
            decompilation(node->son[0], file);
            fprintf(file, "[");
            printSymbol(node, file);
            decompilation(node->son[1], file);
            fprintf(file, "]");
            decompilation(node->son[2], file);
            fprintf(file, ";\n");
            break;
	}

}
