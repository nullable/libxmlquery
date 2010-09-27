%{
#include <stdio.h>
#include "y.tab.h"
%}

%x IN_STATE TEXT_BLOCK CDATA

%%


"<![CDATA["                                     { printf("'ICDATA'"); BEGIN CDATA; }
\n						;
[^<\n]*						{ printf("'TEXT_OUT: %s'", yytext); return TEXT; }
\<						{ printf("'<'"); BEGIN IN_STATE; return START_EL; }


<IN_STATE>\>					{ printf("'>'"); BEGIN INITIAL; return END_EL; }
<IN_STATE>\?					{ printf("'?'"); return '?'; }
<IN_STATE>\/					{ printf("'/'"); return SLASH; }
<IN_STATE>\=					{ printf("'='"); return '='; }
<IN_STATE>[-_a-zA-Z][-_a-zA-Z0-9]*		{ printf("'WORD: %s'", yytext); return WORD; }
<IN_STATE>\"					{ printf("'\"'"); BEGIN TEXT_BLOCK; return '"'; }
<IN_STATE>[\r\n\s]+                  		;

<TEXT_BLOCK>[^"]*				{printf("'TEXT_TB: %s'", yytext); return TEXT; }
<TEXT_BLOCK>\"					{ printf("'\"'"); BEGIN IN_STATE; return '"'; }

<CDATA>("\n"|.)+/"]]>"                          { printf("'CDATA: %s'", yytext); return CDATA_TOK; }
<CDATA>"]]>"					{ printf("'ECDATA'"); BEGIN INITIAL; }
%%


