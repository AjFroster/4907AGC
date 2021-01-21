%option noyywrap

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	int lineno = 1; // initialize to 1
	void ret_print(char *token_type);
	void yyerror();
%}

%x ML_COMMENT

alpha 		[a-zA-Z]
digit 		[0-9]
alnum 		{alpha}|{digit}
print		[ -~]

ID 			{alpha}+{alnum}*
ICONST		"0"|[0-9]{digit}*
FCONST		"0"|{digit}*"."{digit}+
CCONST		(\'{print}\')|(\'\\[nftrbv]\')
STRING		\"{print}*\"

%%

"//".*										{ printf("Eat up comment at line %d\n", lineno); } 

"/*"										{ printf("Eat up comment from line %d ", lineno); BEGIN(ML_COMMENT); }
<ML_COMMENT>"*/" 							{ printf("to line %d\n", lineno); BEGIN(INITIAL); }
<ML_COMMENT>[^*\n]+		
<ML_COMMENT>"*"			
<ML_COMMENT>"\n"							{ lineno += 1; }


"op_IF"										{ ret_print("KEYWORD_IF"); }
"op_ELSE"									{ ret_print("KEYWORD_ELSE"); }
"op_MERGE"									{ ret_print("KEYWORD_MERGE"); }	
"op_EXPANSION"								{ ret_print("KEYWORD_EXPANSION"); }					

"op_INPUT"|"op_OUTPUT"						{ ret_print("IOOP"); }
"op_PLUS"									{ ret_print("ADDOP"); }
"op_TIMES"									{ ret_print("MULOP"); }
"op_MINUS"									{ ret_print("SUBOP"); }
"op_ISEQUAL"|"op_ISLESS"|"op_ISGREATER" 	{ ret_print("RELOP"); }
"MAP_IN"|"MAP_OUT"							{ ret_print("MAPOP"); }



"("											{ ret_print("LPAREN"); }
")"											{ ret_print("RPAREN"); }
"]"											{ ret_print("RBRACK"); }
"["											{ ret_print("LBRACK"); }
"{"											{ ret_print("LBRACE"); }
"}"											{ ret_print("RBRACE"); }
";"											{ ret_print("SEMI"); }
"."											{ ret_print("DOT"); }
","											{ ret_print("COMMA"); }


{ID} 										{ ret_print("ID"); }
{ICONST} 									{ ret_print("ICONST"); }
{FCONST} 									{ ret_print("FCONST"); }
{CCONST} 									{ ret_print("CCONST"); }
{STRING} 									{ ret_print("STRING"); }


"\n"										{ lineno += 1; }
[ \t\r\f]+									/* eat up whitespace */

.											{ yyerror("Unrecognized character"); }

%%

void ret_print(char *token_type){
	printf("yytext: %s\ttoken: %s\tlineno: %d\n", yytext, token_type, lineno);
}

void yyerror(char *message){
	printf("Error: \"%s\" in line %d. Token = %s\n", message, lineno, yytext);
	exit(1);
}

int main(int argc, char *argv[]){
	yyin = fopen(argv[1], "r");
	yylex();
	fclose(yyin);
	return 0;
}