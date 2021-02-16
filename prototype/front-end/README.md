## Files needed
lexer.l 

## To build and run
1.  flex lexer.l                                    | generates lex.yy.c - the source file (logic) for our lexer
2.  bison -d parser.y                               | generates parser.tab.h, and parser.tab.c - the source file for our parser
3.  gcc -o agc parser.tab.c lex.yy.c -lfl           | compiles the generated logic (parser.tab.c and lex.yy.c) into a c executable. The lex.yy.c file is linked with the lfl library
4.  ./agc example_agp                               | run our compiler (agc) using an example file
