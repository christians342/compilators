
int main(){
    int tok;
    hashInit();
    while(running){
        tok = yylex();

        if(!running)
            break;

        switch(tok){
            case KW_INT:
                fprintf(stderr, "Found Keyword 'int'.\n");
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
            case 5:
                fprintf(stderr, "Found new line...\n");
                break;
            case 6:
                fprintf(stderr, "Ignoring...\n");
                break;
            case 7:
                fprintf(stderr, "comment. Ignoring...\n");
                break;
            default:
                fprintf(stderr, "Token ascii %c.\n", tok);
        }
    }
    hashPrint();
}