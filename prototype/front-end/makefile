CC=gcc
CFLAGS=-I.
DEPS = symboltable.h
OBJ = symboltable.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

agc: $(OBJ) lexer.l parser.y
	bison -d parser.y
	flex lexer.l
	$(CC) -o $@ parser.tab.c lex.yy.c -lfl
	rm lex.yy.c parser.tab.c
	
