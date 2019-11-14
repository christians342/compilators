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

#define TAC_IFZ      7
#define TAC_LABEL    8
#define TAC_JUMP     9
#define TAC_READ     10
#define TAC_GREATER  11
#define TAC_LESSER   12
#define TAC_OR       13
#define TAC_AND      14
#define TAC_NOT      15
#define TAC_LE       16
#define TAC_GE       17
#define TAC_EQUAL    18
#define TAC_DIF      19
#define TAC_RET      20
#define TAC_PRINT    21
#define TAC_VEC      22
#define TAC_VECEXP   23
#define TAC_BEGINFUN 24
#define TAC_ENDFUN   25
#define TAC_FUNCCALL 26
#define TAC_ARGPUSH  27
#define TAC_PARAMPOP 28
#define TAC_FOR      29
#define TAC_BREAK    30


#define TAC_JUMPFOR  32
#define TAC_EXP      33



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
TAC* generateCode(AST *ast, HASH_NODE* loopLabel, HASH_NODE* jumpLabel);
TAC* tacJoin(TAC* tac1, TAC* tac2);

#endif