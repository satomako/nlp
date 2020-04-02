/* nlp_variable.c
 * 2017-04-30
 */
#include <stdlib.h>
#include <string.h>
#include "nlp_state.h"
#include "nlp_util.h"
#include "nlp_variable.h"

static struct nlp_variable_list_t *nlp_variable_list;

char *nlp_variable_get_listname()
{
	return nlp_variable_list->name;
}


/*
 * Return value:
 *   = 0 ... case sensitive
 *   = 1 ... ignore case
 */
int nlp_variable_get_opt_case()
{
	return (nlp_variable_list->option & NLP_CASE_MASK) == NLP_IGNORE_CASE ? 1 : 0;
}



/*
 * Return value:
 *   = 0 ... match
 *   = 1 ... unmatch
 */
int nlp_variable_compare_listname(char *name)
{
fprintf(stderr, "need name = [%s]   input name = [%s]\n", nlp_variable_list->name, name);
	if ((nlp_variable_list->option & NLP_CASE_MASK) == NLP_IGNORE_CASE) {
		if (strcasecmp(nlp_variable_list->name, name) == 0) {
			return 0;
		}
	} else {
		if (strcmp(nlp_variable_list->name, name) == 0) {
			return 0;
		}
	}
	return 1;
}



int nlp_variable_set_list(struct nlp_variable_list_t *variable_list)
{
	if (variable_list == NULL) return 1;
	nlp_variable_list = variable_list;
	return 0;
}



int nlp_variable_init_list(struct nlp_variable_list_t *variable_list, char *name, int option)
{
	if (variable_list == NULL || name == NULL) return 1;
//	if (caseopt != NLP_FOLLOW_CASE && caseopt != NLP_IGNORE_CASE) return 1;
	memset(variable_list, 0, sizeof(struct nlp_variable_list_t));
	t_strlcpy(variable_list->name, name, NLP_MAX_IDENTIFIER_LENGTH+1);
	variable_list->option = option;
	return 0;
}



int nlp_variable_free_list(struct nlp_variable_list_t *variable_list)
{
	if (variable_list == NULL) return 1;
	free(variable_list->variable);
	variable_list->variable = NULL;
	return 0;
}



int nlp_variable_set(struct nlp_variable_t *variable, char *name, int type, int option, int value_length, int dimension, int *min, int *max, void *p)
{
	int dim;

	if (variable == NULL) return 1;
	if (type < NLP_VARTYPE_MIN || type > NLP_VARTYPE_MAX) return 1;
	if (dimension < 0 || dimension > NLP_MAX_DIMENSION) return 1;
	if (dimension > 0) {
		if (min == NULL) return 1;
		if (max == NULL) return 1;
	}

	memset(variable, 0, sizeof(struct nlp_variable_t));
	t_strlcpy(variable->name, name, NLP_MAX_IDENTIFIER_LENGTH+1);
	variable->type = type;
	variable->option = option;
	variable->value_length = value_length;
	variable->dimension = dimension;
	variable->array_size = 1;
	for (dim = 0; dim < dimension; dim++) {
		variable->dimension_range_minimum[dim] = min[dim];
		variable->dimension_range_maximum[dim] = max[dim];
		variable->array_size *= max[dim] - min[dim] + 1;
	}
	variable->p = p;
	return 0;
}



int nlp_variable_append(struct nlp_variable_list_t *variable_list, struct nlp_variable_t *variable)
{
	struct nlp_variable_t *tmp_variable = NULL;
	int i;

	if (variable_list == NULL || variable == NULL) return 1;

	for (i = 0; i < variable_list->number; i++) {
		if (strcmp(variable_list->variable[i].name, variable->name) == 0) return 1;
	}

	tmp_variable = malloc(sizeof(struct nlp_variable_t) * (variable_list->number + 1));
	if (tmp_variable == NULL) return 1;
	memcpy(tmp_variable, variable_list->variable, sizeof(struct nlp_variable_t) * variable_list->number);
	memcpy(&tmp_variable[variable_list->number], variable, sizeof(struct nlp_variable_t));
	free(variable_list->variable);
	variable_list->variable = tmp_variable;
	variable_list->number++;
	return 0;
}



/*
 * 
 */
struct nlp_variable_t *nlp_variable_search(char *name)
{
	int i;

	if (name == NULL) return NULL;
	for (i = 0; i < nlp_variable_list->number; i++) {
		if ((nlp_variable_list->option & NLP_CASE_MASK) == NLP_FOLLOW_CASE) {
			if (strcmp(nlp_variable_list->variable[i].name, name) == 0) {
				return &nlp_variable_list->variable[i];
			}
		} else {
			if (strcasecmp(nlp_variable_list->variable[i].name, name) == 0) {
				return &nlp_variable_list->variable[i];
			}
		}
	}
	return NULL;
}
