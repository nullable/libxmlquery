%{
#include <stdio.h>
#include "y.tab.h"
#include "../data_structures/stack.h"

stack* trash_bin;
int lxq_parser_last_state;
%}

%x IN_STATE TEXT_BLOCK CDATA COMMENT FUNC PARAMS NOT_STATE XML_INITIAL QUERY_INITIAL TEXT_BLOCK_DQ_QUERY TEXT_BLOCK_SQ_QUERY REGEX_BLOCK ESCAPE

%%
\@                                                          {BEGIN QUERY_INITIAL; return '@';}
[^@]                                                        {unput(*yytext); BEGIN XML_INITIAL;}

<XML_INITIAL>"<!--"                                         { BEGIN COMMENT; lxq_parser_last_state = XML_INITIAL;}
<XML_INITIAL>"<![CDATA["                                    { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); BEGIN CDATA; }
<XML_INITIAL>\n                                             ;
<XML_INITIAL>[^<\n]*                                        { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<XML_INITIAL>\<                                             { BEGIN IN_STATE; return START_EL; }


<IN_STATE>"<!--"                                            { BEGIN COMMENT; lxq_parser_last_state = IN_STATE;}
<IN_STATE>\>                                                { BEGIN XML_INITIAL; return END_EL; }
<IN_STATE>\?                                                { return '?'; }
<IN_STATE>\/                                                { return SLASH; }
<IN_STATE>\=                                                { return '='; }
<IN_STATE>[-_a-zA-Z][-_a-zA-Z0-9]*                          { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return WORD; }
<IN_STATE>\:                                                { return ':'; }
<IN_STATE>\"                                                { BEGIN TEXT_BLOCK; return '"'; }
<IN_STATE>[\r\n\s]+                                         ;

<TEXT_BLOCK>[^"]*                                           { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<TEXT_BLOCK>\"                                              { BEGIN IN_STATE; return '"'; }

<CDATA>("\n"|.)+/"]]>"                                      { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return CDATA_TOK; }
<CDATA>"]]>"                                                { BEGIN XML_INITIAL; }

<COMMENT>[\n.]*"-->"                                        { BEGIN lxq_parser_last_state; return; }

<QUERY_INITIAL>\*                                           { return ALL; }
<QUERY_INITIAL>[\n\r]+                                      ;
<QUERY_INITIAL>[-_a-zA-Z][-_a-zA-Z0-9]*                     { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return WORD; }
<QUERY_INITIAL>\[                                           { return '['; }
<QUERY_INITIAL>\]                                           { return ']'; }
<QUERY_INITIAL>[ ]*                                         { return SPACE; }
<QUERY_INITIAL>\"                                           { BEGIN TEXT_BLOCK_DQ_QUERY; return '"'; }
<QUERY_INITIAL>\'                                           { BEGIN TEXT_BLOCK_SQ_QUERY; return '\''; }
<QUERY_INITIAL>\/                                           { BEGIN REGEX_BLOCK; return '/'; }
<QUERY_INITIAL>\:                                           { BEGIN FUNC; return ':'; }
<QUERY_INITIAL>[ ]*\,[ ]*                                   { return ','; }
<QUERY_INITIAL>[ ]*\>[ ]*                                   { return '>'; }
<QUERY_INITIAL>[ ]*\+[ ]*                                   { return '+'; }
<QUERY_INITIAL>[ ]*\~[ ]*                                   { return '~'; }
<QUERY_INITIAL>\(                                           { BEGIN PARAMS; return '('; }
<QUERY_INITIAL>\)                                           { return ')'; }
<QUERY_INITIAL>[ ]*=[ ]*                                    { return EQUAL_OP; }
<QUERY_INITIAL>[ ]*"~="[ ]*                                 { return WSSV_OP; }
<QUERY_INITIAL>[ ]*"^="[ ]*                                 { return STARTSW_OP; }
<QUERY_INITIAL>[ ]*"$="[ ]*                                 { return ENDSW_OP; }
<QUERY_INITIAL>[ ]*"*="[ ]*                                 { return CONTAINS_OP; }
<QUERY_INITIAL>[ ]*"|="[ ]*                                 { return DSV_OP; }
<QUERY_INITIAL>[ ]*"!="[ ]*                                 { return NOTEQUAL_OP; }
<QUERY_INITIAL>.                                            { return yytext[0];}

<FUNC>"nth-child"                                           { BEGIN QUERY_INITIAL; return NTH_CHILD_FILTER; }
<FUNC>"nth-last-child"                                      { BEGIN QUERY_INITIAL; return NTH_LAST_CHILD_FILTER; }
<FUNC>"firts-child"                                         { BEGIN QUERY_INITIAL; return FIRST_CHILD_FILTER; }
<FUNC>"last-child"                                          { BEGIN QUERY_INITIAL; return LAST_CHILD_FILTER; }
<FUNC>"only-child"                                          { BEGIN QUERY_INITIAL; return ONLY_CHILD_FILTER; }
<FUNC>"empty"                                               { BEGIN QUERY_INITIAL; return EMPTY_FILTER; }
<FUNC>"not"                                                 { BEGIN NOT_STATE; return NOT_FILTER; }


<NOT_STATE>\(                                               { BEGIN QUERY_INITIAL; return '('; }

<PARAMS>[1-9][0-9]*                                         { yylval.digits = atoi(yytext); return DIGITS; }
<PARAMS>"n"                                                 { return 'n'; }
<PARAMS>[\n\r ]+                                            ;
<PARAMS>\+                                                  { return '+'; }
<PARAMS>\-                                                  { return '-'; }
<PARAMS>"even"                                              { return EVEN; }
<PARAMS>"odd"                                               { return ODD; }
<PARAMS>\)                                                  { BEGIN QUERY_INITIAL; return ')'; }

<REGEX_BLOCK>[^/\\]*                                        { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string);printf("regex:'%s'\n", yytext);  return REGEX; }
<REGEX_BLOCK>\\[^/]                                         { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string);printf("escaped character:'%s'\n", yytext);  return REGEX; }
<REGEX_BLOCK>"/"                                            { BEGIN QUERY_INITIAL; return '/'; }
<REGEX_BLOCK>\\\/                                           { yylval.string = "/"; printf("escaped slash: '%s'\n", yytext); return REGEX; }

<TEXT_BLOCK_SQ_QUERY>[^']*                                  { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<TEXT_BLOCK_SQ_QUERY>\'                                     { BEGIN QUERY_INITIAL; return '\''; }

<TEXT_BLOCK_DQ_QUERY>[^"]*                                  { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
<TEXT_BLOCK_DQ_QUERY>\"                                     { BEGIN QUERY_INITIAL; return '"'; }

<ESCAPE>.                                                   { yylval.string = strdup(yytext); push_stack(trash_bin, yylval.string); return TEXT; }
%%

