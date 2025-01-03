%{
#include <stdio.h>
#include "nlp.h"
//#define YYLEX_PARAM &yylval, &yylloc
extern char *yytext;
%}

%locations
%token	EQ LP RP CM NUMMUL MULMUL
%token	INTEGER REAL STRING SKIPPER
%token	IDENTIFIER SEPARATOR DOT SM
%token	HEAD TAIL TERM
%token	BAD

%%

namelist            : HEAD list TAIL
                    {
                        YYACCEPT;
                    }
                    ;

list                : /* empty */
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:list.empty: %s\n", yytext);
#endif
                    }
                    | let
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:list.let: %s\n", yytext);
#endif
                    }
                    | list let
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:list.list let: %s\n", yytext);
#endif
                    }
                    ;

let                 : variable EQ value.list
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:let: %s\n", yytext);
#endif
                    }
                    ;

variable            : IDENTIFIER
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:variable.IDENTIFIER: %s\n", yytext);
#endif
                    }
                    | IDENTIFIER suffix
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:variable.IDENTIFIER suffix: %s\n", yytext);
#endif
                    }
                    | variable DOT variable
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:variable DOT variable: %s\n", yytext);
#endif
                    }
                    ;

suffix              : LP addr.list RP
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:suffix: %s\n", yytext);
#endif
                    }
                    ;

addr.list           : INTEGER
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:addr.list INTEGER: %s\n", yytext);
#endif
                    }
                    | addr.list CM INTEGER
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:addr.list CM INTEGER: %s\n", yytext);
#endif
                    }
                    ;

/* value.list          : scalar
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:value.list scalar: %s\n", yytext);
#endif
                    }
                    | CM
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:value.list CM: %s\n", yytext);
#endif
                    }
                    | LP value.list RP
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:value.list LP value.list RP: %s\n", yytext);
#endif
                    }
                    | multiplier LP value.list RP
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:value.list multiplier LP value.list RP: %s\n", yytext);
#endif
                    }
                    | value.list scalar
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:value.list value.list scalar: %s\n", yytext);
#endif
                    }
                    | value.list CM
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:value.list value.list CM: %s\n", yytext);
#endif
                    }
                    | value.list LP value.list RP
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:value.list value.list LP value.list RP: %s\n", yytext);
#endif
                    }
                    | value.list multiplier LP value.list RP
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:value.list multiplier LP value.list RP: %s\n", yytext);
#endif
                    }
                    ; */

value.list          : value.elements
                    | LP value.elements RP
                    | multiplier LP value.elements RP
                    | value.list LP value.elements RP
                    | value.list multiplier LP value.elements RP
                    ;

value.elements      : value.element
                    | CM
                    | SKIPPER
                    | value.elements value.element
                    | value.elements CM
                    | value.elements SKIPPER
                    ;

value.element       : scalar
                    | multiplier scalar
                    ;

scalar              : INTEGER
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:scalar INTEGER: %s\n", yytext);
#endif
                    }
                    | REAL
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:scalar REAL: %s\n", yytext);
#endif
                    }
                    | STRING
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:scalar STRING: %s\n", yytext);
#endif
                    }
                    ;

multiplier          : NUMMUL
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:multiplier NUMMUL: %s\n", yytext);
#endif
                    }
                    | MULMUL
                    {
#ifdef __DEBUG_YACC__
                        printf("YACC:multiplier MULMUL: %s\n", yytext);
#endif
                    }
                    ;
