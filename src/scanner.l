    //Scanner:
    //- Identify Lexemas
    //- Classify as Tokens

%{
    #include "astree.h"
    #include "hash.h"
    #include "y.tab.h"

    int lineNumber;
%}

%x COMMENT

DIGIT     [0-9]
ALPHA     [A-Za-z_]
ALPHA_NUM [A-Za-z_0-9]

%%

"byte"                                                  return KW_BYTE;                
"int"                                                   return KW_INT;                
"long"                                                  return KW_LONG;                
"float"                                                 return KW_FLOAT;               
"bool"                                                  return KW_BOOL;                
"if"                                                    return KW_IF;                  
"then"                                                  return KW_THEN;                
"else"                                                  return KW_ELSE;                
"while"                                                 return KW_WHILE;               
"for"                                                   return KW_FOR;                 
"read"                                                  return KW_READ;                
"print"                                                 return KW_PRINT;               
"return"                                                return KW_RETURN;              
"break"                                                 return KW_BREAK;               

"<="                                                    return OPERATOR_LE;            
">="                                                    return OPERATOR_GE;            
"=="                                                    return OPERATOR_EQ;            
"!="                                                    return OPERATOR_DIF;           

"TRUE"                                                  {yylval.symbol =  hashInsert(SYMBOL_LITBOOL, "TRUE", DATATYPE_BOOL);  return LIT_TRUE;}              
"FALSE"                                                 {yylval.symbol =  hashInsert(SYMBOL_LITBOOL, "FALSE", DATATYPE_BOOL);  return LIT_FALSE;}              

[v\,\;\:\(\)\[\]\{\}\+\-\*\/\<\>\=\.\~\&\$\#]           return yytext[0];        


"\n"                                                    ++lineNumber;                            
[ \t]                           
"//".*                          

"###"                                                   BEGIN(COMMENT);                
<COMMENT>"###"                                          BEGIN(INITIAL);                
<COMMENT>"\n"                                           ++lineNumber;                  
<COMMENT>.                      

{ALPHA}{ALPHA_NUM}*             {yylval.symbol =  hashInsert(SYMBOL_IDENTIFIER, yytext, DATATYPE_IDENTIFIER);  return TK_IDENTIFIER;} 
{DIGIT}+                        {yylval.symbol = hashInsert(SYMBOL_LITINT, yytext, DATATYPE_INT);   
                                 return LIT_INTEGER;                                           }
{DIGIT}*"."{DIGIT}+             {yylval.symbol = hashInsert(SYMBOL_LITREAL, yytext, DATATYPE_FLOAT);      
                                return LIT_FLOAT;                                              }
{DIGIT}+"."{DIGIT}*             {yylval.symbol = hashInsert(SYMBOL_LITREAL, yytext, DATATYPE_FLOAT);      
                                return LIT_FLOAT;                                              }

\'([^'\\\n]?|\\.)\'             {yylval.symbol = hashInsert(SYMBOL_LITCHAR, yytext, DATATYPE_CHAR);       return LIT_CHAR;          }
\"([^\"\\\n]|\\.)*\"            {yylval.symbol = hashInsert(SYMBOL_LITSTRING, yytext, DATATYPE_STRING);     return LIT_STRING;        }

%%

int running = 1;

void initMe(void){
    lineNumber = 1;
    hashInit();
}

int getLineNumber(void){   
    return lineNumber;
}

int isRunning(void){
   return running;
}

int yywrap(){
    running = 0;
    return 1;
}

#include "y.tab.c"