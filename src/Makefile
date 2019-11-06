#
# UFRGS - Compiladores B - Amanda Goveia e Christian Schmitz - 2019/2 - Etapa 2
#
# Makefile for single compiler call

etapa3: y.tab.o lex.yy.o main.o hash.o astree.o semantic.o tac.o
	gcc -o etapa4 lex.yy.o main.o hash.o astree.o semantic.o tac.o
lex.yy.o: lex.yy.c y.tab.c
	gcc -c  lex.yy.c  
y.tab.o: y.tab.c
	gcc -c y.tab.c -Wall -ggdb3
hash.o: hash.c
	gcc -c hash.c -Wall -ggdb3
main.o: main.c
	gcc -c main.c -Wall -ggdb3
astree.o: astree.c
	gcc -c astree.c -Wall -ggdb3
semantic.o: semantic.c
	gcc -c semantic.c -Wall -ggdb3
tac.o: tac.c
	gcc -c tac.c -Wall -ggdb3
lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l
y.tab.c: parser.y
	yacc -d parser.y

clean:
	rm lex.yy.c y.tab.c y.tab.h etapa4 *.o
