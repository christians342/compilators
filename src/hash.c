#include "hash.h"

HASH_NODE*Table[HASH_SIZE];

void hashInit(void){
    int i;
    for(i = 0; i < HASH_SIZE; ++i){
        Table[i] = 0;
    }
}

int hashAddress(char *text){
    int address = 1;
    int i;
    for (i = 0; i < strlen(text); ++i){
        address = (address * text[i]) % HASH_SIZE + 1;
    }
    return address - 1;
}

HASH_NODE *hashFind(char *text){
    return 0;
}

HASH_NODE *hashInsert(char *text){
    HASH_NODE *newNode;
    int address = hashAddress(text);
    newNode = (HASH_NODE *) calloc(1, sizeof(HASH_NODE));
    newNode->type = 1;
    newNode->text = (char *) calloc(strlen(text) + 1, sizeof(char));
    strcpy(newNode->text, text);
    newNode->next = Table[address];
    Table[address] = newNode;

    return newNode;
}

void hashPrint(void){
    int i;
    HASH_NODE *node;
    for(i=0; i<HASH_SIZE; ++i)
        for(node=Table[i]; node; node = node->next)
            printf("Table[%d] has %s\n", i, node->text);
}