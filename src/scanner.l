    //Scanner:
    //- Identify Lexemas
    //- Classify as Tokens

%{
    #include "tokens.h"
    #include "hash.c"

    int lineNumber;
%}

%x COMMENT

DIGIT     [0-9]
ALPHA     [A-Za-z_]
ALPHA_NUM [A-Za-z_0-9]

%%

"byte"                          {return KW_BYTE;                }
"int"                           {return KW_INT;                 }
"long"                          {return KW_LONG;                }
"float"                         {return KW_FLOAT;               }
"bool"                          {return KW_BOOL;                }
"if"                            {return KW_IF;                  }
"then"                          {return KW_THEN;                }
"else"                          {return KW_ELSE;                }
"while"                         {return KW_WHILE;               }
"for"                           {return KW_FOR;                 }
"read"                          {return KW_READ;                }
"print"                         {return KW_PRINT;               }
"return"                        {return KW_RETURN;              }
"break"                         {return KW_BREAK;               }

"<="                            {return OPERATOR_LE;            }
">="                            {return OPERATOR_GE;            }
"=="                            {return OPERATOR_EQ;            }
"!="                            {return OPERATOR_DIF;           }

"TRUE"                          {return LIT_TRUE;               }
"FALSE"                         {return LIT_FALSE;              }

[v\,\;\:\(\)\[\]\{\}\+\-\*\/\<\>\=\.\~\&\$\#]      {return yytext[0];        }


"\n"                            {++lineNumber;                  }          
[ \t]                           {}
"//".*                          {}

"###"                           {BEGIN(COMMENT);                }
<COMMENT>"###"                  {BEGIN(INITIAL);                }
<COMMENT>"\n"                   {++lineNumber;                  }
<COMMENT>.                      {}

{ALPHA}{ALPHA_NUM}*             {hashInsert(TK_IDENTIFIER, yytext);  return TK_IDENTIFIER;     } 
{DIGIT}+                        {hashInsert(LIT_INTEGER, yytext);    return LIT_INTEGER;       }
{DIGIT}*"."{DIGIT}+             {hashInsert(LIT_FLOAT, yytext);      return LIT_FLOAT;         }
{DIGIT}+"."{DIGIT}*             {hashInsert(LIT_FLOAT, yytext);      return LIT_FLOAT;         }

\'([^'\\\n]?|\\.)\'             {hashInsert(LIT_CHAR, yytext);       return LIT_CHAR;          }
\"([^\"\\\n]|\\.)*\"            {hashInsert(LIT_STRING, yytext);     return LIT_STRING;        }

\'([^'\\\n]?|\\.)+\'            {return TOKEN_ERROR;            }
.				{return TOKEN_ERROR;            }

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

#include "main.c"
