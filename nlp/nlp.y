%define api.pure full
%locations
%param { yyscan_t scanner }

%code top{
#include <stdio.h>
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
    char *str;
};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif
}
%code requires {
    typedef void* yyscan_t;
}
%code
{
#include "nlp.h"
    int yylex(YYSTYPE *yylvalp, YYLTYPE *yyllocp, yyscan_t scanner);
    void yyerror(YYLTYPE *yyllocp, yyscan_t dummy, const char *message);
}

%union
{
    char *str;
}

%token	EQ LP RP CM NUMMUL MULMUL
%token	INTEGER REAL STRING SKIPPER
%token	IDENTIFIER DOT
%token	HEAD TAIL

%%

namelist            : HEAD lets TAIL
                    {
                        YYACCEPT;
                    }
                    ;

lets                : /* empty */
                    {
                    }
                    | let
                    {
                    }
                    | lets let
                    {
                    }
                    ;

let                 : variable EQ value.list
                    {
                    }
                    ;

variable            : IDENTIFIER
                    {
                    }
                    | IDENTIFIER suffix
                    {
                    }
                    | variable DOT variable
                    {
                    }
                    ;

suffix              : LP addr.list RP
                    {
                    }
                    ;

addr.list           : INTEGER
                    {
                    }
                    | addr.list CM INTEGER
                    {
                    }
                    ;

value.list          : scalar
                    {
                    }
                    | CM
                    {
                    }
                    | LP value.list RP
                    {
                    }
                    | multiplier LP value.list RP
                    {
                    }
                    | value.list scalar
                    {
                    }
                    | value.list CM
                    {
                    }
                    | value.list LP value.list RP
                    {
                    }
                    | value.list multiplier LP value.list RP
                    {
                    }
                    | value.list SKIPPER
                    {
                    }
                    ;

scalar              : INTEGER
                    {
                    }
                    | REAL
                    {
                    }
                    | STRING
                    {
                    }
                    ;

multiplier          : NUMMUL
                    {
                    }
                    | MULMUL
                    {
                    }
                    ;
