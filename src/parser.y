
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "lex.yy.h"
    #include "hash.h"
    #include "astree.h"

    #define SYMBOL_LIT_INT              1
    #define SYMBOL_LIT_REAL             2
    #define SYMBOL_LIT_TRUE             3
    #define SYMBOL_LIT_FALSE            4
    #define SYMBOL_LIT_CHAR             5
    #define SYMBOL_LIT_STRING           6
    #define SYMBOL_IDENTIFIER           7

    int getLineNumber();
    int yyerror(char *message);
    
    AST *root;
  
%}

%union
{
        HASH_NODE *symbol;
        AST *ast;
}



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

%token<symbol> TK_IDENTIFIER
%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_TRUE
%token<symbol> LIT_FALSE
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%type<ast> exp
%type<ast> scalar
%type<ast> cmd
%type<ast> lcmd
%type<ast> block
%type<ast> cmdrest

%type<ast> ldecl
%type<ast> decl
%type<ast> type
%type<ast> scalarNoBool
%type<ast> arrayInit
%type<ast> listLit
%type<ast> parameter
%type<ast> parameterList
%type<ast> rest
%type<ast> printList
%type<ast> expParam
%type<ast> expParamRest
%type<ast> program


%left '+' '-'
%left '*' '/'

%%



program:   
          ldecl                            {$$=$1; astreePrint($1, 0);}       
        ;

ldecl:
          decl ldecl                       {$$=astreeCreate(AST_LDECL, 0, $1, $2, 0, 0);}
         |                                 {$$=0;}
         ;

decl:   
          type TK_IDENTIFIER '(' parameterList ')' cmd         {$$=astreeCreate(AST_FUNC, $2, $1, $4, $6, 0);}    
        | type TK_IDENTIFIER '=' scalar ';'                    {$$=astreeCreate(AST_ATRIB, $2, $1, $4, 0, 0);}
        | type TK_IDENTIFIER '[' LIT_INTEGER ']' arrayInit ';' {$$=astreeCreate(AST_VEC, $2, $1, astreeCreate(AST_SYMBOL, $4, 0, 0, 0, 0), $6, 0);}
        ; 

type:     
          KW_BOOL                          {$$=astreeCreate(AST_BOOL, 0, 0, 0, 0, 0);}
        | KW_BYTE                          {$$=astreeCreate(AST_BYTE, 0, 0, 0, 0, 0);}
        | KW_INT                           {$$=astreeCreate(AST_INT, 0, 0, 0, 0, 0);}
        | KW_LONG                          {$$=astreeCreate(AST_LONG, 0, 0, 0, 0, 0);}
        | KW_FLOAT                         {$$=astreeCreate(AST_FLOAT, 0, 0, 0, 0, 0);}
        ;

scalar:    
          LIT_FLOAT                        {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        | LIT_INTEGER                      {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        | LIT_CHAR                         {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        | LIT_TRUE                         {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        | LIT_FALSE                        {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        ;
        
scalarNoBool:
          LIT_INTEGER                      {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        | LIT_FLOAT                        {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        | LIT_CHAR                         {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        ;
      

arrayInit: 
           ':' scalarNoBool listLit        {$$=astreeCreate(AST_LIT, 0, $2, $3, 0, 0);}
        |                                  {$$=0;}
        ;

listLit: 
            scalarNoBool listLit           {$$=astreeCreate(AST_LLIT, 0, $1, $2, 0, 0);}
        |                                  {$$=0;}
        ;

parameter:    
           type TK_IDENTIFIER              {$$=astreeCreate(AST_PARAM, $2, $1, 0, 0, 0);}
        ;

parameterList:    
           parameter rest                  {$$=astreeCreate(AST_LPARAM, 0, $1, $2, 0, 0);}
        |                                  {$$=0;}
        ;

rest:       
           ',' parameterList               {$$=astreeCreate(AST_RPARAM, 0, $2, 0, 0, 0);}
        |                                  {$$=0;}
        ;


block:      
           '{' lcmd '}'                    {$$=0;}
        ;

printList: 
            LIT_STRING printList          {$$=astreeCreate(AST_LPRINT, 0, astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0), $2, 0, 0);}   
        |   exp printList                 {$$=astreeCreate(AST_EPRINT, 0, $1, $2, 0, 0);}
        |                                 {$$=0;}
        ;

cmd:     
           TK_IDENTIFIER '=' exp                        {$$=astreeCreate(AST_ASS, $1, $3, 0, 0, 0); }
        |  TK_IDENTIFIER '[' exp ']' '=' exp            {$$=astreeCreate(AST_VECEXP, $1, $3, $6, 0, 0);}
        |  KW_READ TK_IDENTIFIER                        {$$=astreeCreate(AST_READ, 0, astreeCreate(AST_SYMBOL, $2, 0, 0, 0, 0), 0, 0, 0);}
        |  KW_PRINT printList                           {$$=astreeCreate(AST_PRINT, 0, $2, 0, 0, 0);}
        |  KW_RETURN exp                                {$$=astreeCreate(AST_RET, 0, $2, 0, 0, 0);}
        |  KW_IF '(' exp ')' KW_THEN cmd KW_ELSE cmd    {$$=astreeCreate(AST_IFELSE, 0, $3, $6, $8, 0);}
        |  KW_IF '(' exp ')' KW_THEN cmd                {$$=astreeCreate(AST_IF, 0, $3, $6, 0, 0);}
        |  KW_WHILE '(' exp ')' cmd                     {$$=astreeCreate(AST_WHILE, 0, $3, $5, 0, 0);}
        |  KW_FOR '(' TK_IDENTIFIER ':' exp ',' exp ',' exp ')' cmd {$$=astreeCreate(AST_FOR, $3, $5, $7, $9, $11);}
        |  KW_BREAK                                     {$$=astreeCreate(AST_BREAK, 0, 0, 0, 0, 0);}
        |  block                                        {$$=astreeCreate(AST_BLOCK, 0, $1, 0, 0, 0);}
        |                                               {$$=0;}
        ;           

lcmd:       
        cmd cmdrest                             {$$=astreeCreate(AST_LCMD, 0, $1, $2, 0, 0);}
        ;

cmdrest:    
        ';' cmd cmdrest                         {$$=astreeCreate(AST_RCMD, 0, $2, $3, 0, 0);}
        |                                       {$$=0;}
        ;


expParam:
           exp expParamRest                    {$$=astreeCreate(AST_EPARAM, 0, $1, $2, 0, 0);}
        ;

expParamRest:       
           ',' expParam                        {$$=astreeCreate(AST_EPARAMREST, 0, $2, 0, 0, 0);}
        |                                      {$$=0;}
        ;

exp:        
           LIT_FLOAT                           {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        |  LIT_INTEGER                         {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        |  LIT_CHAR                            {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        |  TK_IDENTIFIER                       {$$=astreeCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
        |  scalar                              {$$=$1;}                            
        | '(' exp ')'                          {$$=astreeCreate(AST_EXP, 0, $2, 0, 0, 0);}
        |  TK_IDENTIFIER '[' exp ']'           {$$=astreeCreate(AST_VECREAD, $1, $3, 0, 0, 0);}
        |  TK_IDENTIFIER '(' expParam ')'      {$$=astreeCreate(AST_IDEXP, $1, $3, 0, 0, 0);}
        |  exp '+' exp                         {$$=astreeCreate(AST_ADD, 0, $1, $3, 0, 0);}
        |  exp '-' exp                         {$$=astreeCreate(AST_SUB, 0, $1, $3, 0, 0);}
        |  exp '*' exp                         {$$=astreeCreate(AST_MUL, 0, $1, $3, 0, 0);}
        |  exp '/' exp                         {$$=astreeCreate(AST_DIV, 0, $1, $3, 0, 0);}
        |  exp '>' exp                         {$$=astreeCreate(AST_GREATER, 0, $1, $3, 0, 0);}
        |  exp '<' exp                         {$$=astreeCreate(AST_LESSER, 0, $1, $3, 0, 0);}
        |  exp 'v' exp                         {$$=astreeCreate(AST_OR, 0, $1, $3, 0, 0);}
        |  exp '.' exp                         {$$=astreeCreate(AST_AND, 0, $1, $3, 0, 0);}
        |  exp '~' exp                         {$$=astreeCreate(AST_NOT, 0, $1, $3, 0, 0);}
        |  exp OPERATOR_LE exp                 {$$=astreeCreate(AST_LE, 0, $1, $3, 0, 0);}
        |  exp OPERATOR_GE exp                 {$$=astreeCreate(AST_GE, 0, $1, $3, 0, 0);}
        |  exp OPERATOR_EQ exp                 {$$=astreeCreate(AST_EQUAL, 0, $1, $3, 0, 0);}
        |  exp OPERATOR_DIF exp                {$$=astreeCreate(AST_DIF, 0, $1, $3, 0, 0);}
        ;
%%

int yyerror(char *msg){
    fprintf(stderr, "Syntax error at line %d! \n", getLineNumber());
    exit(3);
}

