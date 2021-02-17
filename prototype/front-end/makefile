agc: lexer.l parser.y
	bison -d parser.y
	flex lexer.l
	gcc -o $@ parser.tab.c lex.yy.c -lfl
	rm lex.yy.c parser.tab.c
	
