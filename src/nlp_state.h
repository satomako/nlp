/* nlp_state.h
 * 2017-05-04
 */

#ifndef  _NLP_STATE_H_
#define  _NLP_STATE_H_

#include <stdio.h>
#include "nlp_define.h"

void nlp_init_namelist_name();
void nlp_set_namelist_name(char *name);
char *nlp_get_namelist_name();

void nlp_set_error(int err);
int nlp_get_error();
void nlp_set_state(int state);
int nlp_get_state();
void nlp_set_current_value_type(int type);
int nlp_get_current_value_type();
void nlp_set_previous_value_type(int type);
int nlp_get_previous_value_type();


void nlp_set_addr_dimension(int dim);
int nlp_get_addr_dimension();
void nlp_incr_addr_dimension();
void nlp_set_addr(int dim, int addr);
int nlp_get_addr(int dim);


void nlp_reset_string_length();
void nlp_incr_string_length();
int nlp_get_string_length();
void nlp_set_string_char(char chr);
char *nlp_get_string();

int yywrap();
int yyparse();
void yyerror(const char *str);

#endif /*_NLP_STATE_H_*/
