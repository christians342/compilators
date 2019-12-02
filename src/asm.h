#ifndef ASM_HEADER
#define ASM_HEADER

#import "hash.h"
#import "astree.h"
#import "tac.h"

void generateASM(TAC* tac, FILE* fout);
void generateASMVariables(AST* node, FILE* fout);
void generateASMTemps(FILE* fout);
void generateASMLiterals(FILE* fout);

#endif