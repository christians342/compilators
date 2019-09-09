
%{
    #include <stdio.h>
    #include <stdlib.h>

    #define SYMBOL_LIT_INT              1
    #define SYMBOL_LIT_REAL             2
    #define SYMBOL_LIT_TRUE             3
    #define SYMBOL_LIT_FALSE            4
    #define SYMBOL_LIT_CHAR             5
    #define SYMBOL_LIT_STRING           6
    #define SYMBOL_IDENTIFIER           7

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

program:   
          program decl
        |
        ;

decl:   
          fundec 
        | globaldec
        ; 

type:     
          KW_BOOL 
        | KW_BYTE 
        | KW_INT 
        | KW_LONG 
        | KW_FLOAT
        ;

scalar:     
          LIT_TRUE 
        | LIT_FALSE
        | LIT_INTEGER
        | LIT_FLOAT  
        | LIT_CHAR 
        ;


globaldec: 
           type TK_IDENTIFIER '=' scalar ';'
        |  type TK_IDENTIFIER '[' LIT_INTEGER ']' arrayInit ';'
        ;

arrayInit: 
           ':' scalar listLit
        |   
        ;

listLit: 
            scalar listLit
        |  
        ;

fundec:     
           type TK_IDENTIFIER '(' parameterList ')' cmd
        ;

parameter:    
           type TK_IDENTIFIER
        ;

parameterList:    
           parameter rest
        |
        ;

rest:       
           ',' parameterList
        |
        ;

//comandos

block:      
           '{' lcmd '}'
        ;

printList: 
            LIT_STRING printList         
        |   exp printList
        |
        ;

simpleCmd:
           TK_IDENTIFIER '=' exp 
        |  TK_IDENTIFIER '[' exp ']' '=' listLit 
        |  KW_READ TK_IDENTIFIER
        |  KW_READ scalar
        |  KW_PRINT printList
        |  KW_RETURN exp
        ;

fluxControl:
           KW_IF '(' exp ')' KW_THEN cmd KW_ELSE cmd
        |  KW_IF '(' exp ')' KW_THEN cmd
        |  KW_WHILE '(' exp ')' cmd
        |  KW_FOR '(' TK_IDENTIFIER ':' exp ',' exp ',' exp ')' cmd
        |  KW_BREAK
        ;

cmd:     
           simpleCmd
        |  block
        |  fluxControl
        |
        ;
           

lcmd:       
        cmd cmdrest
        ;

cmdrest:    
        ';' cmd cmdrest
        |
        ;


//expressões

op:
           '+'
        |  '-'
        |  '*'
        |  '/'
        |  '>'
        |  '<'
        |  'v'
        |  '.'
        |  '~'
        |  OPERATOR_LE   
        |  OPERATOR_GE   
        |  OPERATOR_EQ   
        |  OPERATOR_DIF
        ; 

expParam:
           exp expParamRest
        ;

expParamRest:       
           ',' expParam
        |
        ;

exp:
            TK_IDENTIFIER
         |  scalar
         | '(' exp ')'
         |  TK_IDENTIFIER '[' exp ']'
         |  TK_IDENTIFIER '(' expParam ')' 
         |  exp op exp
         |
         ;
%%

int yyerror(char *msg){
    fprintf(stderr, "Syntax error at line %d! \n", getLineNumber());
    exit(3);
}
