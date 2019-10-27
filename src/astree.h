#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#define AST_SYMBOL 1
#define AST_ADD 2 //*
#define AST_MUL 3 //*
#define AST_SUB 4 //*
#define AST_DIV 5 //*
#define AST_VECREAD 6
#define AST_ASS 7
#define AST_LCMD 8
#define AST_BOOL 9
#define AST_BYTE 10
#define AST_INT 11
#define AST_LONG 12
#define AST_FLOAT 13
#define AST_EXP 14
#define AST_FUNC 15
#define AST_VARDEC 16
#define AST_LIT 17
#define AST_PARAM 18
#define AST_LPARAM 19
#define AST_RPARAM 20
#define AST_LPRINT 21
#define AST_EPRINT 22
#define AST_VECEXP 23
#define AST_READ 24
#define AST_RET 25
#define AST_IFELSE 26
#define AST_IF 27
#define AST_WHILE 28 
#define AST_FOR 29
#define AST_BREAK 30
#define AST_BLOCK 31
#define AST_RCMD 32
#define AST_EPARAM 33
#define AST_EPARAMREST 34
#define AST_IDEXP 35
#define AST_GREATER 36 //*
#define AST_LESSER 37 //*
#define AST_OR 38 //*
#define AST_AND 39 //*
#define AST_NOT 40 //*
#define AST_LE 41 //*
#define AST_GE 42 //*
#define AST_EQUAL 43 //*
#define AST_DIF 44 //*
#define AST_LDECL 45
#define AST_VEC 46
#define AST_LLIT 47
#define AST_FIRST_LLIT 48
#define AST_PRINT 49

#define MAX_SONS 4

typedef struct astree_node{
    int type;
    int line;
    HASH_NODE *symbol;
    int datatype;
    struct astree_node *son[MAX_SONS];
} AST;

AST *astreeCreate(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3, int line);
void astreePrint(AST *node, int level);
void decompilation(AST* node, FILE *output);
AST *getRoot();

#endif
