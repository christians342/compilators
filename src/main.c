
int main(){
    int tok;
    hashInit();
    while(running){
        tok = yylex();

        if(!running)
            break;

        switch(tok){
            case KW_BYTE:
                fprintf(stderr, "Found Keyword 'byte'.\n");
                break;
            case KW_INT:
                fprintf(stderr, "Found Keyword 'int'.\n");
                break;
            case KW_LONG:
                fprintf(stderr, "Found Keyword 'long'.\n");
                break;
            case KW_FLOAT:
                fprintf(stderr, "Found Keyword 'float'.\n");
                break;
            case KW_BOOL:
                fprintf(stderr, "Found Keyword 'bool'.\n");
                break;
            case KW_IF:
                fprintf(stderr, "Found Keyword 'if'.\n");
                break;
            case KW_THEN:
                fprintf(stderr, "Found Keyword 'then'.\n");
                break;
            case KW_ELSE:
                fprintf(stderr, "Found Keyword 'else'.\n");
                break;
            case KW_WHILE:
                fprintf(stderr, "Found Keyword 'while'.\n");
                break;
            case KW_FOR:
                fprintf(stderr, "Found Keyword 'for'.\n");
                break;
            case KW_READ:
                fprintf(stderr, "Found Keyword 'read'.\n");
                break;
            case KW_PRINT:
                fprintf(stderr, "Found Keyword 'print'.\n");
                break;
            case KW_RETURN:
                fprintf(stderr, "Found Keyword 'return'.\n");
                break;
            case KW_BREAK:
                fprintf(stderr, "Found Keyword 'break'.\n");
                break;
            case TK_IDENTIFIER:
                fprintf(stderr, "Identifier.\n");
                break;
            case LIT_INTEGER:
                fprintf(stderr, "Literal Integer.\n");
                break;
            case LIT_FLOAT:
                fprintf(stderr, "Literal Real.\n");
                break;
            case LIT_TRUE:
                fprintf(stderr, "Literal TRUE.\n");
                break;
            case LIT_FALSE:
                fprintf(stderr, "Literal FALSE.\n");
                break;
            case OPERATOR_LE:
                fprintf(stderr, "Operator le.\n");
                break;
            case OPERATOR_GE:
                fprintf(stderr, "Operator ge.\n");
                break;
            case OPERATOR_EQ:
                fprintf(stderr, "Operator eq.\n");
                break;
            case OPERATOR_DIF:
                fprintf(stderr, "Operator dif.\n");
                break;
            default:
                fprintf(stderr, "Token ascii %c.\n", tok);
        }
    }
    hashPrint();
}