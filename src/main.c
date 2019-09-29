#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "astree.h"

#define ERROR_CODE 3

int yyparse();
extern FILE* yyin;

int main(int argc, char** argv){

    FILE *output;

    if (argc < 3){
        printf("Please call: ./etapa3 input.txt output.txt\n");
        exit(ERROR_CODE);
    }

    yyin = fopen(argv[1], "r");

    if (yyin == NULL) {
        printf("Unable to open file %s.\n", argv[1]);
        exit(ERROR_CODE);
    }

    output = fopen(argv[2], "w+");

    if (output == NULL) {
        printf("Unable to open file %s.\n", argv[1]);
        exit(ERROR_CODE);
    }

    hashInit();
    yyparse();
    hashPrint();
    decompilation(getRoot(),output);

    fclose(output);

    exit(0);
}