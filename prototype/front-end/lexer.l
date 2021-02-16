%option noyywrap

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "symboltable.h"
	#include "parser.tab.h"
	extern FILE *yyin;
	extern FILE *yyout;

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


"op_IF"										{ return TOKEN_IF; }
"op_ELSE"									{ return TOKEN_ELSE; }
"op_MERGE"									{ return TOKEN_MERGE; }	
"op_EXPANSION"								{ return TOKEN_EXPANSION; }
"EXPAND"									{ return TOKEN_EXPAND; }
"DATUM"										{ return TOKEN_DATUM; }
"OPERATOR"									{ return TOKEN_OPERATOR; }
"SUBGRAPH"									{ return TOKEN_SUBGRAPH; }
"INPUT"										{ return TOKEN_INPUT; }
"OUTPUT"									{ return TOKEN_OUTPUT; } 	
"CONST"										{ return TOKEN_CONST; }	
"MAP_IN"									{ return TOKEN_MAPIN; }	
"MAP_OUT"									{ return TOKEN_MAPOUT; }	
		
"op_INPUT"									{ return IOP; }
"op_OUTPUT"									{ return OOP; }
"op_PLUS"									{ return ADDOP; }
"op_TIMES"									{ return MULOP; }
"op_MINUS"									{ return SUBOP; }
"op_ISEQUAL"|"op_ISLESS"|"op_ISGREATER" 	{ return RELOP; }

"("											{ return LPAR; }
")"											{ return RPAR; }
"]"											{ return RBRACK; }
"["											{ return LBRACK; }
"{"											{ return LBRACE; }
"}"											{ return RBRACE; }
";"											{ return SEMI; }
"."											{ return DOT; }
","											{ return COMMA; }


{ID} 										{ 
											insert(yytext, strlen(yytext), UNDEF, lineno);
											yylval.symtab_item = lookup(yytext);
											return ID; 
											}
{ICONST}    								{ yylval.int_val   = atoi(yytext); return ICONST; }
{FCONST}    								{ yylval.double_val = atof(yytext); return FCONST; }
{CCONST}    								{ yylval.char_val  = yytext[0];    return CCONST; }
{STRING}    								{ yylval.str_val = malloc(yyleng * sizeof(char));
								              strcpy(yylval.str_val, yytext);  return STRING; }

"\n"										{ lineno += 1; }
[ \t\r\f]+									/* eat up whitespace */

.											{ yyerror("Unrecognized character"); }

%%
