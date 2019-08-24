    //Scanner:
    //- Identify Lexemas
    //- Classify as Tokens

%{
    #include "tokens.h"
    #include "hash.c"

    int lineNumber = 1;
%}

%x COMMENT

DIGIT [0-9]

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

"TRUE"                          {return LIT_TRUE;               }
"FALSE"                         {return LIT_FALSE;              }

[A-Za-z_][A-Za-z_0-9]*          {return TK_IDENTIFIER;          } 

{DIGIT}+                        {return LIT_INTEGER;            }
{DIGIT}+"."{DIGIT}+             {return LIT_FLOAT;              }

"<="                            {return OPERATOR_LE;            }
">="                            {return OPERATOR_GE;            }
"=="                            {return OPERATOR_EQ;            }
"!="                            {return OPERATOR_DIF;           }

[,;:()[]\{\}+-*/<>=v.~&$#]      {return yytext[0];              }

"\n"                            {++lineNumber;                  }          
[ \t]                           {}
"//".*                          {}

"###"                           {BEGIN(COMMENT);                }
<COMMENT>"###"                  {BEGIN(INITIAL);                }
<COMMENT>"\n"                   {++lineNumber;                  }
<COMMENT>.                      {}

%%

int running = 1;

int yywrap(){
    running = 0;
    return 1;
}

#include "main.c"