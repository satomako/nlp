/* nlp_state.c
 * 2017-05-04
 *
 * 
 */

#include <stdio.h>
#include <string.h>
#include "nlp_util.h"
#include "nlp_state.h"
#include "nlp_log.h"

/* Namelist name */
static char nlp_namelist_name[NLP_MAX_STRING_LENGTH];

/* State */
static int nlp_state;
static int nlp_current_value_type;
static int nlp_previous_value_type;
static int nlp_error;

/* Address */
static int nlp_addr_dimension;
static int nlp_addr[NLP_MAX_DIMENSION];

/* String */
static int nlp_string_length;
static char nlp_string[NLP_MAX_STRING_LENGTH];

/* YACC */
extern FILE *yyin;
extern char *yytext;
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
extern YYLTYPE yylloc;



void nlp_set_error(int err)
{
	nlp_error = err;
}



int nlp_get_error()
{
	return nlp_error;
}



void nlp_init_namelist_name()
{
	memset(nlp_namelist_name, 0, sizeof(nlp_namelist_name));
}



void nlp_set_namelist_name(char *name)
{
	nlp_init_namelist_name();
	t_strlcpy(nlp_namelist_name, &name[1], sizeof(nlp_namelist_name));
}



char *nlp_get_namelist_name()
{
	return nlp_namelist_name;
}



void nlp_set_state(int state)
{
	nlp_state = state;
}



int nlp_get_state()
{
	return nlp_state;
}



void nlp_set_current_value_type(int type)
{
	nlp_previous_value_type = nlp_current_value_type;
	nlp_current_value_type = type;
}



int nlp_get_current_value_type()
{
	return nlp_current_value_type;
}



void nlp_set_previous_value_type(int type)
{
	nlp_previous_value_type = type;
}



int nlp_get_previous_value_type()
{
	return nlp_previous_value_type;
}



void nlp_set_addr_dimension(int dim)
{
	nlp_addr_dimension = dim;
}



int nlp_get_addr_dimension()
{
	return nlp_addr_dimension;
}



void nlp_incr_addr_dimension()
{
	nlp_addr_dimension++;
}



void nlp_set_addr(int dim, int addr)
{
	nlp_addr[dim - 1] = addr;
}



int nlp_get_addr(int dim)
{
	return nlp_addr[dim - 1];
}



void nlp_reset_string_length()
{
	nlp_string_length = 0;
}



void nlp_incr_string_length()
{
	nlp_string_length++;
}



int nlp_get_string_length()
{
	return nlp_string_length;
}



void nlp_set_string_char(char chr)
{
	nlp_string[nlp_string_length] = chr;
}



char *nlp_get_string()
{
	return nlp_string;
}



int yywrap()
{
	return 1;
}



void yyerror(const char *str)
{
	nlp_set_error(NLP_ERR_ERROR);
	fprintf(stderr, "error: %s: last token = %s\n", str, yytext);
	fprintf(stderr, "  pos=%d %d %d %d\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column);
}
