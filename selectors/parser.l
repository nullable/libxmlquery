%{
#include <stdio.h>
#include "y.tab.h"

// <FUNC>.*					{ fprintf(stderr, "Error: syntax error before '%s', pseudo-selector expected\n", yytext); exit(1); }
// <FUNC>[-_a-zA-Z][-_a-zA-Z0-9]*			{ fprintf(stderr, "Error: '%s' pseudo-selector is not supported\n", yytext); exit(1); }
%}

%x IN_STATE TEXT_BLOCK CDATA FUNC PARAMS NOT_STATE

%%


\*						{ return ALL; };
[\n\r]+						;
[-_a-zA-Z][-_a-zA-Z0-9]*			{ yylval.string = strdup(yytext); return WORD; }
\[						{ return '['; }
\]						{ return ']'; }
[ ]*						{ return SPACE; }
\,						{ return ','; }
\"						{ BEGIN TEXT_BLOCK; return '"'; }
\:						{ BEGIN FUNC; return ':'; }
[ ]*\>[ ]*					{ return '>'; }
[ ]*\+[ ]*						{ return '+'; }
[ ]*\~[ ]*						{ return '~'; }
\(						{ BEGIN PARAMS; return '('; }
\)						{ return ')'; }
[ ]*\=[ ]*						{ return EQUAL_OP; }
[ ]*"~\="[ ]*						{ return WSSV_OP; }
[ ]*"^\="[ ]*						{ return STARTSW_OP; }
[ ]*"$\="[ ]*						{ return ENDSW_OP; }
[ ]*"*\="[ ]*						{ return CONTAINS_OP; }
[ ]*"|\="[ ]*						{ return DSV_OP; }
[ ]*"!\="[ ]*						{ return NOTEQUAL_OP; }


<FUNC>"nth-child"				{ BEGIN INITIAL; return NTH_CHILD_FILTER; }
<FUNC>"nth-last-child"				{ BEGIN INITIAL; return NTH_LAST_CHILD_FILTER; }
<FUNC>"firts-child"				{ BEGIN INITIAL; return FIRST_CHILD_FILTER; }
<FUNC>"last-child"				{ BEGIN INITIAL; return LAST_CHILD_FILTER; }
<FUNC>"only-child"				{ BEGIN INITIAL; return ONLY_CHILD_FILTER; }
<FUNC>"empty"					{ BEGIN INITIAL; return EMPTY_FILTER; }
<FUNC>"not"					{ BEGIN NOT_STATE; return NOT_FILTER; }


<NOT_STATE>\(						{ BEGIN INITIAL; return '('; }

<PARAMS>[1-9][0-9]*				{ yylval.digits = atoi(yytext); return DIGITS; }
<PARAMS>"n"					{ return 'n'; } 
<PARAMS>[\n\r ]+				;
<PARAMS>\+					{ return '+'; }
<PARAMS>\-					{ return '-'; }
<PARAMS>"even"					{ return EVEN; }
<PARAMS>"odd"					{ return ODD; }
<PARAMS>\)					{ BEGIN INITIAL; return ')'; }


<TEXT_BLOCK>[^"]*				{ yylval.string = strdup(yytext); return TEXT; }
<TEXT_BLOCK>\"					{ BEGIN INITIAL; return '"'; }

%%


