%{
#include <stdio.h>
#include "y.tab.h"
#include "../data_structures/stack.h"

stack* trash_bin;
%}

%x IN_STATE TEXT_BLOCK CDATA COMMENT

%%

"<!--"                                          {printf("'COMMENT'"); BEGIN COMMENT;}  
"<![CDATA["                                     { printf("'ICDATA'"); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); BEGIN CDATA; }
\n						;
[^<\n]*						{ printf("'TEXT_OUT: %s'", yytext); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
\<						{ printf("'<'"); BEGIN IN_STATE; return START_EL; }


<IN_STATE>\>					{ printf("'>'"); BEGIN INITIAL; return END_EL; }
<IN_STATE>\?					{ printf("'?'"); return '?'; }
<IN_STATE>\/					{ printf("'/'"); return SLASH; }
<IN_STATE>\=					{ printf("'='"); return '='; }
<IN_STATE>[-_a-zA-Z][-_a-zA-Z0-9]*		{ printf("'WORD: %s'", yytext); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return WORD; }
<IN_STATE>\"					{ printf("'\"'"); BEGIN TEXT_BLOCK; return '"'; }
<IN_STATE>[\r\n\s]+                  		;

<TEXT_BLOCK>[^"]*				{printf("'TEXT_TB: %s'", yytext); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<TEXT_BLOCK>\"					{ printf("'\"'"); BEGIN IN_STATE; return '"'; }

<CDATA>("\n"|.)+/"]]>"                          { printf("'CDATA: %s'", yytext); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return CDATA_TOK; }
<CDATA>"]]>"					{ printf("'ECDATA'"); BEGIN INITIAL; }

<COMMENT>[\n.]*"-->"                            { printf("'COMMENT: %s ENDCOMMENT'", yytext); BEGIN INITIAL;}
%%


