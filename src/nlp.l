%top{
/*FIXME*/
#include <stdint.h>
}

%option never-interactive

%{
/* nlp.l
 * 2017-04-30
 * tabs:8
 */

#define YY_NO_UNISTD_H

#include <stdio.h>
#include <stdlib.h>
#include "nlp_state.h"
#include "nlp_variable.h"
#include "nlp_value.h"
#include "nlp_log.h"
#include "nlp.tab.c"

int yylineno;
int yycolumn;

#define YY_USER_ACTION {yylloc.first_line = yylineno; \
        yylloc.first_column = yycolumn;               \
        yycolumn=yycolumn+yyleng;                     \
        yylloc.last_column=yycolumn;                  \
        yylloc.last_line = yylineno;}

%}

ND		[a-zA-Z_]
NM		[1-9]
DG		[0-9]
AD		[0-9a-zA-Z_]
SN		[+-]
EP		[deDE]{SN}?{DG}+
FC		{DG}*"."{DG}+
FC2		{DG}+"."
%start		SSTR
%start		DSTR
%start		COMMENT


%%
<INITIAL>&[eE][nN][dD]			{
						if (nlp_get_state() != NLP_STATE_CONTAINS &&
							nlp_get_state() != NLP_STATE_TERM) {
							return BAD;
						}
						nlp_set_state(NLP_STATE_INITIAL);
						return TAIL;
					}
<INITIAL>&{ND}{AD}{0,254}		{
						if (nlp_get_state() != NLP_STATE_INITIAL) {
							return BAD;
						}
						nlp_set_namelist_name(yytext);
						if (nlp_variable_compare_listname(nlp_get_namelist_name()) == 1) {
							nlp_log("ERROR: invalid namelist name(%s)\n"
								"       requrement [&%s]\n"
								"            input [&%s]\n",
								nlp_variable_get_opt_case() == 1 ? "ignore case" : "follow case",
								nlp_variable_get_listname(),
								nlp_get_namelist_name());
							return BAD;
						}
						nlp_set_state(NLP_STATE_CONTAINS);
						return HEAD;
					}
<INITIAL>\/				{
						nlp_set_state(NLP_STATE_TERM);
						return TERM;
					}
<INITIAL>"="				{
						nlp_set_current_value_type(NLP_VALTYPE_NULL);
						nlp_set_state(NLP_STATE_VALUES);
						return EQ;
					}
<INITIAL>"("				{
						if (nlp_get_state() == NLP_STATE_IDENTIFIER) {
							nlp_set_state(NLP_STATE_ADDR);
						} else if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_LP);
							nlp_value_append(NLP_VALTYPE_LP, "(");
						}
						return LP;
					}
<INITIAL>")"				{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_RP);
							nlp_value_append(NLP_VALTYPE_RP, ")");
						}
						return RP;
					}
<INITIAL>","				{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_NULL);
							if (nlp_get_previous_value_type() == NLP_VALTYPE_NULL ||
								nlp_get_previous_value_type() == NLP_VALTYPE_LP) {
								nlp_value_append(NLP_VALTYPE_NULL, ",");
							}
						}
						return CM;
					}
<INITIAL>[ \t]				;
<INITIAL>\n				{
						yycolumn = 1;
						yylineno++;
					}
<INITIAL>!.*\n				;	/* comment */
<INITIAL>#.*\n				;	/* comment */
<INITIAL>\/\*				{
						BEGIN COMMENT;
					}
<INITIAL>{SN}?{DG}+			{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_INTEGER);
							nlp_value_append(NLP_VALTYPE_INTEGER, yytext);
						}
						return INTEGER;
					}
<INITIAL>{NM}{DG}*\*			{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_MULTIPLY);
							nlp_value_append(NLP_VALTYPE_MULTIPLY, yytext);
						}
						return NUMMUL;
					}
<INITIAL>"**"				{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_MULTIPLY);
							nlp_value_append(NLP_VALTYPE_MULTIPLY, yytext);
						}
						return MULMUL;
					}
<INITIAL>{SN}?{DG}+{EP}			{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_REAL);
							nlp_value_append(NLP_VALTYPE_REAL, yytext);
						}
						return REAL;
					}
<INITIAL>{SN}?{FC}+			{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_REAL);
							nlp_value_append(NLP_VALTYPE_REAL, yytext);
						}
						return REAL;
					}
<INITIAL>{SN}?{FC}+{EP}			{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_REAL);
							nlp_value_append(NLP_VALTYPE_REAL, yytext);
						}
						return REAL;
					}
<INITIAL>{SN}?{FC2}+			{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_REAL);
							nlp_value_append(NLP_VALTYPE_REAL, yytext);
						}
						return REAL;
					}
<INITIAL>{SN}?{FC2}+{EP}		{
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_REAL);
							nlp_value_append(NLP_VALTYPE_REAL, yytext);
						}
						return REAL;
					}
<INITIAL>{ND}{AD}*			{
						nlp_set_state(NLP_STATE_IDENTIFIER);
						nlp_value_append(NLP_VALTYPE_IDENTIFIER, yytext);
						return IDENTIFIER;
					}
<INITIAL>@+				{
						nlp_value_append(NLP_VALTYPE_SKIPPER, yytext);
						return SKIPPER;
					}
<INITIAL>\"				{
						nlp_reset_string_length();
						BEGIN DSTR;
					}
<INITIAL>'				{
						nlp_reset_string_length();
						BEGIN SSTR;
					}
<INITIAL><<EOF>>			{
printf("eof detected, current state = %d\n", nlp_get_state());
						yyterminate();
					}
<DSTR>\"				{
						nlp_set_string_char('\0');
						BEGIN INITIAL;
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_STRING);
							nlp_value_append(NLP_VALTYPE_STRING, nlp_get_string());
						}
						return STRING;
					}
<SSTR>'					{
						nlp_set_string_char('\0');
						BEGIN INITIAL;
						if (nlp_get_state() == NLP_STATE_VALUES) {
							nlp_set_current_value_type(NLP_VALTYPE_STRING);
							nlp_value_append(NLP_VALTYPE_STRING, nlp_get_string());
						}
						return STRING;
					}
<DSTR>.					{
						if (nlp_get_string_length() < NLP_MAX_STRING_LENGTH - 2) {
							nlp_set_string_char(yytext[0]);
							nlp_incr_string_length();
						}
					}
<SSTR>.					{
						if (nlp_get_string_length() < NLP_MAX_STRING_LENGTH - 2) {
							nlp_set_string_char(yytext[0]);
							nlp_incr_string_length();
						}
					}
<COMMENT>\*\/				{
						BEGIN INITIAL;
					}
%%
