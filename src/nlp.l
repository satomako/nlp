%top{
/*FIXME*/
#include <stdint.h>
}

%option never-interactive

%{
/* nlp.l
 * 2017-04-30
 * tabs:2
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

#define YY_USER_ACTION { \
          yylloc.first_line = yylineno; \
          yylloc.first_column = yycolumn; \
          yycolumn=yycolumn+yyleng; \
          yylloc.last_line = yylineno; \
          yylloc.last_column=yycolumn; \
        }

struct context_stack_t
{
  struct context_stack_t *prev;
  YY_BUFFER_STATE bs;
  int lineno;
  char *filename;
  FILE *f;
};

static struct context_stack_t *current_context = NULL;
static char *current_filename;

static int nlp_new_context(char *filename);
static int nlp_pop_context();
%}

ND		[a-zA-Z_]
NM		[1-9]
DG		[0-9]
AD		[0-9a-zA-Z_]
SN		[+-]
EP		[deDE]{SN}?{DG}+
FC		{DG}*"."{DG}+
FC2		{DG}+"."

%start NAMEL
%start SSTR
%start DSTR
%start IFILE
%start COMMENT


%%

<NAMEL>&[eE][nN][dD] {
  if (nlp_get_state() != NLP_STATE_CONTAINS && nlp_get_state() != NLP_STATE_TERM)
  {
    return BAD;
  }
  nlp_set_state(NLP_STATE_END);
  return TAIL;
}

<NAMEL>&[sS][tT][oO][pP] {
  if (nlp_get_state() != NLP_STATE_CONTAINS && nlp_get_state() != NLP_STATE_TERM)
  {
    return BAD;
  }
  nlp_set_state(NLP_STATE_STOP);
  return TAIL;
}

<INITIAL>&{ND}{AD}{0,254}	{
  if (nlp_get_state() != NLP_STATE_INITIAL)
  {
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
  BEGIN NAMEL;
  return HEAD;
}

<NAMEL>\/	{
  nlp_set_state(NLP_STATE_TERM);
  return TERM;
}

<NAMEL>"=" {
  nlp_set_current_value_type(NLP_VALTYPE_NULL);
  nlp_set_state(NLP_STATE_VALUES);
  return EQ;
}

<NAMEL>"(" {
  if (nlp_get_state() == NLP_STATE_IDENTIFIER)
  {
    nlp_set_state(NLP_STATE_ADDR);
  } else if (nlp_get_state() == NLP_STATE_VALUES) {
    nlp_set_current_value_type(NLP_VALTYPE_LP);
    nlp_value_append(NLP_VALTYPE_LP, "(");
  }
  return LP;
}

<NAMEL>")" {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_RP);
    nlp_value_append(NLP_VALTYPE_RP, ")");
  }
  return RP;
}

<NAMEL>"," {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_NULL);
    if (nlp_get_previous_value_type() == NLP_VALTYPE_NULL || nlp_get_previous_value_type() == NLP_VALTYPE_LP)
    {
      nlp_value_append(NLP_VALTYPE_NULL, ",");
    }
  }
  return CM;
}

<INITIAL>[ \t] ;
<NAMEL>[ \t] ;
<INITIAL>\n {
  yycolumn = 1;
  yylineno++;
}
<NAMEL>\n {
  yycolumn = 1;
  yylineno++;
}
<COMMENT>\n {
  yycolumn = 1;
  yylineno++;
}
<NAMEL>^#[ \t]*include[ \t]*[<'\"] {
  BEGIN IFILE;
}
<IFILE>[^ \t\n>'\"] {
  {
    int c;
    while ((c = input() && c!= '\n'));
  }
  yylineno++;
  
}
<INITIAL><<EOF>> {
  printf("eof detected, current state = %d\n", nlp_get_state());
  yyterminate();
}
<NAMEL><<EOF>> {
  printf("eof detected, current state = %d\n", nlp_get_state());
  yyterminate();
}
<NAMEL>!.*\n ;	/* comment */
<NAMEL>#.*\n ;	/* comment */
<NAMEL>\/\* {
  BEGIN COMMENT;
}
<COMMENT>\*\/ {
  BEGIN NAMEL;
}
<COMMENT>. ;
<NAMEL>{SN}?{DG}+ {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_INTEGER);
    nlp_value_append(NLP_VALTYPE_INTEGER, yytext);
  }
  return INTEGER;
}
<NAMEL>{NM}{DG}*\* {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_MULTIPLY);
    nlp_value_append(NLP_VALTYPE_MULTIPLY, yytext);
  }
  return NUMMUL;
}
<NAMEL>"**" {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_MULTIPLY);
    nlp_value_append(NLP_VALTYPE_MULTIPLY, yytext);
  }
  return MULMUL;
}
<NAMEL>{SN}?{DG}+{EP} {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_REAL);
    nlp_value_append(NLP_VALTYPE_REAL, yytext);
  }
  return REAL;
}
<NAMEL>{SN}?{FC}+ {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_REAL);
    nlp_value_append(NLP_VALTYPE_REAL, yytext);
  }
  return REAL;
}
<NAMEL>{SN}?{FC}+{EP} {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_REAL);
    nlp_value_append(NLP_VALTYPE_REAL, yytext);
  }
  return REAL;
}
<NAMEL>{SN}?{FC2}+ {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_REAL);
    nlp_value_append(NLP_VALTYPE_REAL, yytext);
  }
  return REAL;
}
<NAMEL>{SN}?{FC2}+{EP} {
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_REAL);
    nlp_value_append(NLP_VALTYPE_REAL, yytext);
  }
  return REAL;
}
<NAMEL>{ND}{AD}* {
  nlp_set_state(NLP_STATE_IDENTIFIER);
  nlp_value_append(NLP_VALTYPE_IDENTIFIER, yytext);
  return IDENTIFIER;
}
<NAMEL>@+ {
  nlp_value_append(NLP_VALTYPE_SKIPPER, yytext);
  return SKIPPER;
}
<NAMEL>\" {
  nlp_reset_string_length();
  BEGIN DSTR;
}
<NAMEL>' {
  nlp_reset_string_length();
  BEGIN SSTR;
}
<DSTR>\" {
  nlp_set_string_char('\0');
  BEGIN NAMEL;
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_STRING);
    nlp_value_append(NLP_VALTYPE_STRING, nlp_get_string());
  }
  return STRING;
}
<SSTR>' {
  nlp_set_string_char('\0');
  BEGIN NAMEL;
  if (nlp_get_state() == NLP_STATE_VALUES)
  {
    nlp_set_current_value_type(NLP_VALTYPE_STRING);
    nlp_value_append(NLP_VALTYPE_STRING, nlp_get_string());
  }
  return STRING;
}
<DSTR>. {
  if (nlp_get_string_length() < NLP_MAX_STRING_LENGTH - 2)
  {
    nlp_set_string_char(yytext[0]);
    nlp_incr_string_length();
  }
}
<SSTR>. {
  if (nlp_get_string_length() < NLP_MAX_STRING_LENGTH - 2)
  {
    nlp_set_string_char(yytext[0]);
    nlp_incr_string_length();
  }
}
%%

char *nlp_get_current_filename()
{
  return current_filename;
}

int nlp_set_context(FILE *f, char *filename)
{
  struct context_stack_t *con;

  if (f == NULL)
  {
    return 1;
  }
  con = malloc(sizeof(struct context_stack_t));
  if (con == NULL)
  {
    return 1;
  }
  con->filename = nlp_filename_list_add(filename);
  if (con->filename == NULL)
  {
    fclose(f);
    free(con);
    return 1;
  }

  con->prev = NULL;
  con->bs = yy_create_buffer(f, YY_BUF_SIZE);
  con->f = f;
  yy_switch_to_buffer(con->bs);
  current_context = con;
  yylineno = 1;
  current_filename = con->filename;
  return 0;
}

static int nlp_new_context(char *filename)
{
  struct context_stack_t *con;

  if (current_context == NULL)
  {
    return 1;
  }
  FILE *f = fopen(filename, "r");
  if (f == NULL)
  {
    return 1;
  }
  con = malloc(sizeof(struct context_stack_t));
  if (con == NULL)
  {
    fclose(f);
    return 1;
  }
  con->filename = nlp_filename_list_add(filename);
  if (con->filename == NULL)
  {
    fclose(f);
    free(con);
    return 1;
  }

  current_context->lineno = yylineno;
  con->prev = current_context;
  con->bs = yy_create_buffer(f, YY_BUF_SIZE);
  con->f = f;
  yy_switch_to_buffer(con->bs);
  current_context = con;
  yylineno = 1;
  current_filename = con->filename;
  return 0;
}

static int nlp_pop_context()
{
  struct context_stack_t *con = current_context;
  struct context_stack_t *prev_con;

  if (con == NULL)
  {
    return 1;
  }
  if (con->prev != NULL)
  {
    fclose(con->f);
  }
  yy_delete_buffer(con->bs);
  prev_con = con->prev;
  free(con);
  if (prev_con == NULL)
  {
    return 0;
  }
  yy_switch_to_buffer(prev_con->bs);
  yylineno = prev_con->lineno;
  current_filename = prev_con->filename;

  current_context = prev_con;
  return 0;
}