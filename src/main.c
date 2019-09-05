int main(int argc, char** argv){
    hashInit();
    yyparse();
    hashPrint();

    fprintf(stderr, "Terrific! Go on! \n");

    exit(0);
}