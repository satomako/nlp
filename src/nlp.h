/* nlp.h
 * 2017-04-30
 */

#ifndef  _NLP_H_
#define  _NLP_H_

#include <stdio.h>
#include "nlp_define.h"
#include "nlp_variable.h"

#ifdef NLP_UNDERSCORE
#define nlp_init nlp_init_
#define nlp_add_variable nlp_add_variable_
#define nlp nlp_
#define nlp_cleanup nlp_cleanup_
#endif /* NLP_UNDERSCORE */

/* Fortran interface */
int nlp_init(char *name, int *caseopt, int len_name);
int nlp_add_variable(char *varname, char *vartype, int *dim, int *min, int *max, void *p, int len_varname, int len_vartype);
int nlp(char *inp_filename, char *log_filename, int len_inp_filename, int len_log_filename);
int nlp_cleanup();

#endif /*_NLP_H_*/
