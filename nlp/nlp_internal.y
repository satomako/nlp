/*
* nlp_internal.y
* parse variable definition string.
*
* grammar
*   variable_definition : variable_type IDENTIFIER [ dimension  ];
*   variable_type       : INT8 | INT32 | INT64 | REAL32 | REAL64 | fstring;
*   fstring             : STRING LP INTEGER RP;
*   dimension           : addr_list;
*   addr_list           : range | addr_list CM range;
*   range               : INTEGER | INTEGER COLON INTEGER;
*/

%{
#include <stdio.h>
extern char *zztext;
%}

%define api.prefix {zz}
%locations
%token INT8 INT32 INT64 REAL32 REAL64 STRING
%token IDENTIFIER INTEGER
%token COLON CM LP RP SM
%token END

%%
all                 : variable_definition_list
                    {
                        YYACCEPT;
                    }
                    ;
variable_definition_list: variable_definition
                    {
                    }
                    | variable_definition_list variable_definition
                    {
                    }
                    ;
variable_definition : variable_type IDENTIFIER dimension SM
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
                    ;

fstring             : STRING LP INTEGER RP
                    {
                    }
                    ;

dimension           : /* empty */
                    {
                    }
                    | LP addr_list RP
                    {
                    }
                    ;

range               : INTEGER
                    {
                    }
                    | INTEGER COLON INTEGER
                    {
                    }

addr_list           : range
                    {
                    }
                    | addr_list CM range
                    {
                    }

%%