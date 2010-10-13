%{
#include <stdio.h>
#include "y.tab.h"
#include "../data_structures/stack.h"

stack* trash_bin;
%}

%x IN_STATE TEXT_BLOCK CDATA COMMENT FUNC PARAMS NOT_STATE XML_INITIAL QUERY_INITIAL TEXT_BLOCK_DQ_QUERY  TEXT_BLOCK_SQ_QUERY

%%
\@                                                           {BEGIN QUERY_INITIAL; return '@';}                                    
[^@]                                                        {unput(*yytext); BEGIN XML_INITIAL;}

<XML_INITIAL>"<!--"                                          {printf("'COMMENT'"); BEGIN COMMENT;}  
<XML_INITIAL>"<![CDATA["                                     { printf("'ICDATA'"); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); BEGIN CDATA; }
<XML_INITIAL>\n						;
<XML_INITIAL>[^<\n]*						{ printf("'TEXT_OUT: %s'", yytext); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<XML_INITIAL>\<						{ printf("'<'"); BEGIN IN_STATE; return START_EL; }


<IN_STATE>\>					{ printf("'>'"); BEGIN XML_INITIAL; return END_EL; }
<IN_STATE>\?					{ printf("'?'"); return '?'; }
<IN_STATE>\/					{ printf("'/'"); return SLASH; }
<IN_STATE>\=					{ printf("'='"); return '='; }
<IN_STATE>[-_a-zA-Z][-_a-zA-Z0-9]*		{ printf("'WORD: %s'", yytext); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return WORD; }
<IN_STATE>\"					{ printf("'\"'"); BEGIN TEXT_BLOCK; return '"'; }
<IN_STATE>[\r\n\s]+                  		;

<TEXT_BLOCK>[^"]*				{printf("'TEXT_TB: %s'", yytext); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<TEXT_BLOCK>\"					{ printf("'\"'"); BEGIN IN_STATE; return '"'; }

<CDATA>("\n"|.)+/"]]>"                          { printf("'CDATA: %s'", yytext); yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return CDATA_TOK; }
<CDATA>"]]>"					{ printf("'ECDATA'"); BEGIN XML_INITIAL; }

<COMMENT>[\n.]*"-->"                            { printf("'COMMENT: %s ENDCOMMENT'", yytext); BEGIN XML_INITIAL;}

<QUERY_INITIAL>\*						{ return ALL; };
<QUERY_INITIAL>[\n\r]+						;
<QUERY_INITIAL>[-_a-zA-Z][-_a-zA-Z0-9]*			{ yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return WORD; }
<QUERY_INITIAL>\[						{ return '['; }
<QUERY_INITIAL>\]						{ return ']'; }
<QUERY_INITIAL>[ ]*						{ return SPACE; }
<QUERY_INITIAL>[ ]*\,[ ]*						{ return ','; }
<QUERY_INITIAL>\"						{ BEGIN TEXT_BLOCK_DQ_QUERY; return '"'; }
<QUERY_INITIAL>\'						{ BEGIN TEXT_BLOCK_SQ_QUERY; return '\''; }
<QUERY_INITIAL>\:						{ BEGIN FUNC; return ':'; }
<QUERY_INITIAL>[ ]*\>[ ]*					{ return '>'; }
<QUERY_INITIAL>[ ]*\+[ ]*						{ return '+'; }
<QUERY_INITIAL>[ ]*\~[ ]*						{ return '~'; }
<QUERY_INITIAL>\(						{ BEGIN PARAMS; return '('; }
<QUERY_INITIAL>\)						{ return ')'; }
<QUERY_INITIAL>[ ]*=[ ]*						{ return EQUAL_OP; }
<QUERY_INITIAL>[ ]*"~="[ ]*						{ return WSSV_OP; }
<QUERY_INITIAL>[ ]*"^="[ ]*						{ return STARTSW_OP; }
<QUERY_INITIAL>[ ]*"$="[ ]*						{ return ENDSW_OP; }
<QUERY_INITIAL>[ ]*"*="[ ]*						{ return CONTAINS_OP; }
<QUERY_INITIAL>[ ]*"|="[ ]*						{ return DSV_OP; }
<QUERY_INITIAL>[ ]*"!="[ ]*						{ return NOTEQUAL_OP; }
<QUERY_INITIAL>.                                                        { return yytext[0];}

<FUNC>"nth-child"				{ BEGIN QUERY_INITIAL; return NTH_CHILD_FILTER; }
<FUNC>"nth-last-child"				{ BEGIN QUERY_INITIAL; return NTH_LAST_CHILD_FILTER; }
<FUNC>"firts-child"				{ BEGIN QUERY_INITIAL; return FIRST_CHILD_FILTER; }
<FUNC>"last-child"				{ BEGIN QUERY_INITIAL; return LAST_CHILD_FILTER; }
<FUNC>"only-child"				{ BEGIN QUERY_INITIAL; return ONLY_CHILD_FILTER; }
<FUNC>"empty"					{ BEGIN QUERY_INITIAL; return EMPTY_FILTER; }
<FUNC>"not"					{ BEGIN NOT_STATE; return NOT_FILTER; }


<NOT_STATE>\(						{ BEGIN QUERY_INITIAL; return '('; }

<PARAMS>[1-9][0-9]*				{ yylval.digits = atoi(yytext); return DIGITS; }
<PARAMS>"n"					{ return 'n'; } 
<PARAMS>[\n\r ]+				;
<PARAMS>\+					{ return '+'; }
<PARAMS>\-					{ return '-'; }
<PARAMS>"even"					{ return EVEN; }
<PARAMS>"odd"					{ return ODD; }
<PARAMS>\)					{ BEGIN QUERY_INITIAL; return ')'; }


<TEXT_BLOCK_SQ_QUERY>[^']*				{ yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<TEXT_BLOCK_SQ_QUERY>\'					{ BEGIN QUERY_INITIAL; return '\''; }

<TEXT_BLOCK_DQ_QUERY>[^"]*				{ yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<TEXT_BLOCK_DQ_QUERY>\"					{ BEGIN QUERY_INITIAL; return '"'; }


%%


