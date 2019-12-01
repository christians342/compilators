#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

HASH_NODE*Table[HASH_SIZE];

void hashInit(void){
   memset(Table, 0, HASH_SIZE * sizeof(HASH_NODE*));
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
    HASH_NODE *node;
    int address = hashAddress(text);

    for(node = Table[address]; node; node = node->next)
        if(!strcmp(text,node->text))
            return node;

    return 0; 
}

HASH_NODE *hashInsert(int type, char *text, int datatype){
    HASH_NODE *newNode;
    int address = hashAddress(text);

     if ((newNode = hashFind(text)) != 0)
        return newNode;

    newNode = (HASH_NODE *) calloc(1, sizeof(HASH_NODE));
    newNode->type = type;
    newNode->text = (char *) calloc(strlen(text) + 1, sizeof(char));
    newNode->datatype = datatype;

    strcpy(newNode->text, text);
    newNode->next = Table[address];
    Table[address] = newNode;

    return newNode;
}

HASH_NODE* makeTemp(int datatype){
    static int serialNumber = 0;
    static char name[100];

    sprintf(name, "__temp%d__", serialNumber++);
    return hashInsert(0, name, datatype);
}

HASH_NODE* makeLabel(char* prefix){
    static int serialNumber = 0;
    static char name[100];

    sprintf(name, "__label_%s_%d__", prefix, serialNumber++);
    return hashInsert(0, name, 0);
}

void hashPrint(void){
    int i;
    HASH_NODE *node;
    for(i=0; i<HASH_SIZE; ++i)
        for(node=Table[i]; node; node = node->next)
            fprintf(stderr," Table[%d] has %s and type %d\n",i,Table[i]->text, Table[i]->datatype);
}

int hashCheckUndeclared(void){
    int i;
    HASH_NODE *node;
    int error = 0;
    for(i=0; i<HASH_SIZE; ++i)
        for(node=Table[i]; node; node = node->next)
            if(node->type == SYMBOL_IDENTIFIER){
                fprintf(stderr,"Semantic ERROR. Undeclared identifier %s\n", Table[i]->text);
                error++;
            }
    return error;
}

HASH_NODE** getTable(){
    return Table;
}
