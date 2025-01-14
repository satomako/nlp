/*
* nlp_internal.y
* parse variable definition string.
*
* grammar
*   all                 : definition_list
*   definition_list     : variable_definition | type_definition | definition_list variable_definition | definition_list type_definition
*   variable_definition_list: variable_definition | variable_definition_list variable_definition
*   variable_definition : variable_type IDENTIFIER [ dimension  ] ;
*   variable_type       : INT8 | INT32 | INT64 | REAL32 | REAL64 | fstring | user_defined_type
*   fstring             : STRING LP INTEGER RP
*   user_defined_type   : TYPE IDENTIFIER [ dimension ]
*   dimension           : addr_list
*   addr_list           : range | addr_list CM range
*   range               : INTEGER | INTEGER COLON INTEGER
*   type_definition     : TYPEDEF IDENTIFIER LB variable_definition_list RB
*/

%define api.prefix {zz}
%define api.pure full
%locations
%param { yyscan_t scanner }

%code top{
#include <stdio.h>
#define ZZDEBUG 1
union YYSTYPE
{
    char *str;
};
typedef union YYSTYPE YYSTYPE;
}
%code requires {
    typedef void* yyscan_t;
}
%code
{
    int zzlex(YYSTYPE *yylvalp, YYLTYPE *yyllocp, yyscan_t scanner);
    void zzerror(YYLTYPE *yyllocp, yyscan_t dummy, const char *message);
}

%union
{
    char *str;
}

%token INT8 INT32 INT64 REAL32 REAL64 STRING TYPE
%token TYPEDEF LB RB
%token IDENTIFIER INTEGER
%token COLON CM LP RP SM
%token END

%%
all                 : definition_list END
                    {
                        YYACCEPT;
                    }
                    ;

definition_list     : variable_definition
                    {
                    }
                    | type_definition
                    {
                    }
                    | definition_list variable_definition
                    {
                    }
                    | definition_list type_definition
                    {
                    }
                    ;

variable_definition : variable_type IDENTIFIER dimension SM
                    {
                    }
                    ;

variable_definition_list: variable_definition
                    {
                    }
                    | variable_definition_list variable_definition
                    {
                    }
                    ;

variable_type       : INT8
                    {
                    }                       
                    | INT32
                    {
                    }                       
                    | INT64
                    {
                    }                       
                    | REAL32
                    {
                    }                       
                    | REAL64
                    {
                    }                       
                    | fstring
                    {
                    }
                    | user_defined_type
                    {
                    }
                    ;

fstring             : STRING LP INTEGER RP
                    {
                    }
                    ;

user_defined_type   : TYPE IDENTIFIER
                    {
                    }
                    ;

dimension           : /* empty */
                    {}
                    | LP addr_list RP
                    {
                    }
                    ;

addr_list           : range
                    {
                    }
                    | addr_list CM range
                    {
                    }
                    ;

range               : INTEGER
                    {
                    }
                    | INTEGER COLON INTEGER
                    {
                    }
                    ;

type_definition     : TYPEDEF IDENTIFIER LB variable_definition_list RB
                    {
                    }
                    ;

%%