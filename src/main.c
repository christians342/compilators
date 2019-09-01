int main(){
    hashInit();
    yyparse();
    hashPrint();

    fprintf(stderr, "Terrific! Go on! \n");

    exit(0);
}