#
# UFRGS - Compiladores B - Amanda Goveia e Christian Schmitz - 2019/2 - Etapa 2
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa2: lex.yy.c y.tab.c y.tab.h
	gcc -o etapa2 lex.yy.c
lex.yy.c: scanner.l
	lex scanner.l
y.tab.c: 
y.tab.h:
	yacc parser.y -d

clean:
	rm lex.yy.c y.tab.c y.tab.h etapa2
