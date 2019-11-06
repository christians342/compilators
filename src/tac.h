#ifndef TAC_HEADER
#define TAC_HEADER

#import "hash.h"
#import "astree.h"

#define TAC_SYMBOL  1
#define TAC_ADD     2
#define TAC_SUB     3
#define TAC_MUL     4
#define TAC_DIV     5
#define TAC_MOVE    6


typedef struct tac_node{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;

    struct tac_node *prev;
    struct tac_node *next;
} TAC;

TAC* tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrintSingle(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC* generateCode(AST *ast);
TAC* tacJoin(TAC* tac1, TAC* tac2);

#endif