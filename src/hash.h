#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>

#define HASH_SIZE           997

#define SYMBOL_IDENTIFIER   1
#define SYMBOL_LITINT       2
#define SYMBOL_LITREAL      3
#define SYMBOL_LITBOOL      4
#define SYMBOL_LITBYTE      5
#define SYMBOL_LITCHAR      6
#define SYMBOL_LITSTRING    7

#define SYMBOL_SCALAR       1001
#define SYMBOL_VECTOR       1002
#define SYMBOL_FUNC         1003

#define DATATYPE_INT        2001
#define DATATYPE_FLOAT      2002
#define DATATYPE_BOOL       2003
#define DATATYPE_BYTE       2004
#define DATATYPE_LONG       2005
#define DATATYPE_STRING     2006
#define DATATYPE_IDENTIFIER 2007
#define DATATYPE_CHAR       2008
#define DATATYPE_ERROR      3000


typedef struct hash_node{
    int type;
    int datatype;
    char *text;
    struct hash_node *next;

} HASH_NODE;

void hashInit(void);

int hashAddress(char *text);
HASH_NODE* hashFind(char *text);
HASH_NODE* hashInsert(int type, char *text, int datatype);
void hashPrint(void);
int hashCheckUndeclared(void);

#endif