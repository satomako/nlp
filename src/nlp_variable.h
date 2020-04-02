/* nlp_variable.h
 * 2017-04-30
 */
#ifndef   _NLP_VARIABLE_H_
#define   _NLP_VARIABLE_H_

#include "nlp_define.h"

#define NLP_VARTYPE_MIN        (1)
#define NLP_VARTYPE_INTEGER    (1)
#define NLP_VARTYPE_INTEGER8   (2)
#define NLP_VARTYPE_REAL       (3)
#define NLP_VARTYPE_REAL8      (4)
#define NLP_VARTYPE_STRING     (5)
#define NLP_VARTYPE_FIXCHAR    (6)
#define NLP_VARTYPE_MAX        (6)

#define NLP_MAX_FIXCHAR_LENGTH (1024)

struct nlp_variable_t {
	char name[NLP_MAX_IDENTIFIER_LENGTH+1];
	int type;
	int option;
	int value_length;
	int dimension;
	int dimension_range_minimum[NLP_MAX_DIMENSION];
	int dimension_range_maximum[NLP_MAX_DIMENSION];
	int array_size;
	void *p;
};

struct nlp_variable_list_t {
	char name[NLP_MAX_IDENTIFIER_LENGTH+1];
	int option;
	int number;
	struct nlp_variable_t *variable;
};

int nlp_variable_compare_listname(char *name);
char *nlp_variable_get_listname();
int nlp_variable_get_opt_case();
int nlp_variable_set_list(struct nlp_variable_list_t *variable_list);
int nlp_variable_init_list(struct nlp_variable_list_t *variable_list, char *name, int caseopt);
int nlp_variable_free_list(struct nlp_variable_list_t *variable_list);
int nlp_variable_set(struct nlp_variable_t *variable, char *name, int type, int option, int value_length, int dimension, int *min, int *max, void *p);
int nlp_variable_append(struct nlp_variable_list_t *variable_list, struct nlp_variable_t *variable);
struct nlp_variable_t *nlp_variable_search(char *name);

#endif /* _NLP_VARIABLE_H_ */
