/*
Copyright (c) 2010 Frederico Gonçalves, Vasco Fernandes

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

%{
#include <stdio.h>
#include "../include/y.tab.h"
#include "../include/stack.h"
#include "../include/byte_buffer.h"

stack* trash_bin = NULL;
byte_buffer* lxq_cdata_buffer = NULL;
int lxq_parser_last_state;

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(*str == ' ') str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && *end == ' ') end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}
%}
%x IN_STATE TEXT_BLOCK_SQ_XML TEXT_BLOCK_DQ_XML CDATA COMMENT FUNC PARAMS NOT_STATE XML_INITIAL QUERY_INITIAL TEXT_BLOCK_DQ_QUERY TEXT_BLOCK_SQ_QUERY REGEX_BLOCK ESCAPE

%option yylineno
%%
\@                                                          {BEGIN QUERY_INITIAL; return '@';}
"<!--"                                                      { BEGIN COMMENT; lxq_parser_last_state = XML_INITIAL;}
"<![CDATA["                                                 { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; BEGIN CDATA; }
\r?\n                                                       {BEGIN XML_INITIAL;}
[^@<\n\r]*                                                  { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; BEGIN XML_INITIAL; return TEXT; }
\<                                                          { BEGIN IN_STATE; return START_EL; }

<XML_INITIAL>"<!--"                                         { BEGIN COMMENT; lxq_parser_last_state = XML_INITIAL; }
<XML_INITIAL>"<![CDATA["                                    { BEGIN CDATA; lxq_cdata_buffer = new_byte_buffer(16); }
<XML_INITIAL>\r?\n                                          ;
<XML_INITIAL>[ \t]*                                         ;
<XML_INITIAL>[^<\r\n\t ]                                    { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return TEXT; }
<XML_INITIAL>[^<\r\n\t ][^<\r\n]*[^<\r\n\t ]                { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return TEXT; }
<XML_INITIAL>\<                                             { BEGIN IN_STATE; return START_EL; }

<IN_STATE>"<!--"                                            { BEGIN COMMENT; lxq_parser_last_state = IN_STATE;}
<IN_STATE>\>                                                { BEGIN XML_INITIAL; return END_EL; }
<IN_STATE>\?                                                { return '?'; }
<IN_STATE>\/                                                { return SLASH; }
<IN_STATE>\=                                                { return '='; }
<IN_STATE>[-_a-zA-Z][-_a-zA-Z0-9]*                          { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return WORD; }
<IN_STATE>\:                                                { return ':'; }
<IN_STATE>\"                                                { BEGIN TEXT_BLOCK_DQ_XML; return '"'; }
<IN_STATE>\'                                                { BEGIN TEXT_BLOCK_SQ_XML; return '\''; }
<IN_STATE>[\r\n \t]+                                        ;

<TEXT_BLOCK_SQ_XML>[^']*                                    { yylval.string = strdup(yytext); return TEXT; }
<TEXT_BLOCK_SQ_XML>\'                                       { BEGIN IN_STATE; return '\''; }

<TEXT_BLOCK_DQ_XML>[^"]*                                    { yylval.string = strdup(yytext); return TEXT; }
<TEXT_BLOCK_DQ_XML>\"                                       { BEGIN IN_STATE; return '"'; }

<CDATA>("\n"|.|"\r")                                        { append_string_to_buffer(yytext, lxq_cdata_buffer); }
<CDATA>"]]>"                                                { append_bytes_to_buffer("\0", lxq_cdata_buffer, 1);
                                                              yylval.string = lxq_cdata_buffer->buffer;
                                                              lxq_cdata_buffer->buffer = NULL;
                                                              destroy_byte_buffer(lxq_cdata_buffer);
                                                              lxq_cdata_buffer = NULL;
                                                              BEGIN XML_INITIAL;
                                                              return CDATA_TOK;

                                                            }

<COMMENT>("\n"|.|"\r")                                           {;}
<COMMENT>"-->"                                              { BEGIN lxq_parser_last_state; }

<QUERY_INITIAL>\*                                           { return ALL; }
<QUERY_INITIAL>[\n\r]+                                      ;
<QUERY_INITIAL>[_a-zA-Z][-_a-zA-Z0-9]*                      { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return WORD; }
<QUERY_INITIAL>[ ]*![_a-zA-Z][-_a-zA-Z0-9]*[ ]*             { yylval.string = strdup(yytext); char* aux = strdup(trimwhitespace(yylval.string));
                                                              free(yylval.string); yylval.string = aux; return CUSTOM_RELATION_OPERATOR; }
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
<QUERY_INITIAL>\.                                           { return '.'; }
<QUERY_INITIAL>\#                                           { return '#'; }
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
<FUNC>"first-child"                                         { BEGIN QUERY_INITIAL; return FIRST_CHILD_FILTER; }
<FUNC>"last-child"                                          { BEGIN QUERY_INITIAL; return LAST_CHILD_FILTER; }
<FUNC>"only-child"                                          { BEGIN QUERY_INITIAL; return ONLY_CHILD_FILTER; }
<FUNC>"empty"                                               { BEGIN QUERY_INITIAL; return EMPTY_FILTER; }
<FUNC>"not"                                                 { BEGIN NOT_STATE; return NOT_FILTER; }
<FUNC>[_a-zA-Z][-_a-zA-Z0-9]*                               { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; BEGIN QUERY_INITIAL; return CUSTOM_FILTER; }



<NOT_STATE>\(                                               { BEGIN QUERY_INITIAL; return '('; }

<PARAMS>[1-9][0-9]*                                         { yylval.digits = atoi(yytext); return DIGITS; }
<PARAMS>"n"                                                 { return 'n'; }
<PARAMS>[\n\r ]+                                            ;
<PARAMS>\+                                                  { return '+'; }
<PARAMS>\-                                                  { return '-'; }
<PARAMS>"even"                                              { return EVEN; }
<PARAMS>"odd"                                               { return ODD; }
<PARAMS>[_a-zA-Z][-_a-zA-Z0-9]*                             { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return WORD; }
<PARAMS>\)                                                  { BEGIN QUERY_INITIAL; return ')'; }

<REGEX_BLOCK>[^/\\]*                                        { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return REGEX; }
<REGEX_BLOCK>\\[^/]                                         { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return REGEX; }
<REGEX_BLOCK>"/i"                                           { BEGIN QUERY_INITIAL; return END_REGEXI; }
<REGEX_BLOCK>"/"                                            { BEGIN QUERY_INITIAL; return '/'; }
<REGEX_BLOCK>\\\/                                           { yylval.string = strdup("/"); return REGEX; }

<TEXT_BLOCK_SQ_QUERY>[^']*                                  { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return TEXT; }
<TEXT_BLOCK_SQ_QUERY>\'                                     { BEGIN QUERY_INITIAL; return '\''; }

<TEXT_BLOCK_DQ_QUERY>[^"]*                                  { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return TEXT; }
<TEXT_BLOCK_DQ_QUERY>\"                                     { BEGIN QUERY_INITIAL; return '"'; }

<ESCAPE>.                                                   { yylval.string = strdup(yytext); /*push_stack(trash_bin, yylval.string)*/; return TEXT; }
%%

