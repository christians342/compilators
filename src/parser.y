
%{
    #include <stdio.h>
    #include <stdlib.h>
%}

%token KW_BYTE       
%token KW_INT        
%token KW_LONG       
%token KW_FLOAT      
%token KW_BOOL       

%token KW_IF         
%token KW_THEN       
%token KW_ELSE       
%token KW_WHILE      
%token KW_FOR        
%token KW_READ       
%token KW_PRINT      
%token KW_RETURN     
%token KW_BREAK      

%token OPERATOR_LE   
%token OPERATOR_GE   
%token OPERATOR_EQ   
%token OPERATOR_DIF  

%token TK_IDENTIFIER 

%token LIT_INTEGER   
%token LIT_FLOAT
%token LIT_TRUE
%token LIT_FALSE
%token LIT_CHAR
%token LIT_STRING

%%

program:   program decl
        |
        ;

decl:        fundec | vardec
        ;

vardec:     KW_INT TK_IDENTIFIER '=' LIT_INTEGER ';'
        ;

fundec:     KW_INT TK_IDENTIFIER '(' parlist ')' cmd
        ;

parlist:    par rest
        |
        ;

rest:       ','par rest
        |
        ;

par:        KW_INT
        ;

block:      '{' lcmd '}'
        ;

cmd:         TK_IDENTIFIER '=' LIT_FLOAT
        |    block
        ;

lcmd:       lcmd cmd ';'
        |
        ;

%%

int yyerror(char *msg){
    fprintf(stderr, "Syntax error at line %d! \n", lineNumber);
    exit(3);
}

// deal with empty command vs empty list