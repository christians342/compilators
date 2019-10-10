#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>

#define HASH_SIZE           997

#define SYMBOL_IDENTIFIER   1
#define SYMBOL_LITINT       2
#define SYMBOL_LITREAL      3

#define SYMBOL_SCALAR       1001
#define SYMBOL_VECTOR       1002
#define SYMBOL_FUNC         1003

typedef struct hash_node{
    int type;
    char *text;
    struct hash_node *next;

} HASH_NODE;

void hashInit(void);

int hashAddress(char *text);
HASH_NODE* hashFind(char *text);
HASH_NODE* hashInsert(int type, char *text);
void hashPrint(void);
void hashCheckUndeclared(void);

#endif