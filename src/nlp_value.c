/* nlp_value.c
 * Create: 2017-04-30
 *
 * List of values linked by nlp.l to bidirectional list.
 * List of values is decoded when the parser has detemined
 * the assignment of a variable.
 */

#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "nlp_define.h"
#include "nlp_state.h"
#include "nlp_value.h"
#include "nlp_variable.h"
#include "nlp_util.h"

/* private global */
static FILE *_log;

/* module */
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;

static struct nlp_value_t nlp_value_head;
static struct nlp_value_t nlp_decode_head;
static int nlp_decode_stack_pointer;
static struct nlp_decode_stack_t nlp_decode_stack[NLP_VALUE_NEST_MAX];
extern int yylineno;
extern YYLTYPE yylloc;


static char *nlp_valtype_str[] = {"NULL", "ID", "INTEGER", "REAL", "STRING", "MULTIPLY", "LP", "RP", "SKIPPER"};

/*
 */
void nlp_value_set_logfile(FILE *logfile)
{
	_log = logfile;
}



/*
 * Purpose: insert value to value-list
 * insert v at dst
 * Arguments:
 *   struct nlp_value_t *dst ... insert position of value-list
 *   struct nlp_value_t *v ... the value to insert
 * Return value:
 *   (none)
 */
void nlp_value_insert(struct nlp_value_t *dst, struct nlp_value_t *v)
{
	v->next = dst->next;
	v->prev = dst;
	dst->next = v;
	v->next->prev = v;
}



/*
 * Purpose: remove value from value-list
 * Arguments:
 *   struct nlp_value_t *v ... the value to remove
 * Return value:
 *   (none)
 */
void nlp_value_remove(struct nlp_value_t *v)
{
	v->prev->next = v->next;
	v->next->prev = v->prev;
	v->next = v;
	v->prev = v;
}



/*
 * Purpose: free value
 * Artuments:
 *   struct nlp_value_t *v ... the value to free
 * Return value:
 *   (none)
 */
void nlp_value_free(struct nlp_value_t *v)
{
	if (v->value) free(v->value);
	free(v);
}



/*
 * Purpose: free value-list
 * Arguments:
 *   struct nlp_value_t *head ... head of value-list to free
 * Return value:
 *   (none)
 */
void nlp_value_list_free(struct nlp_value_t *head)
{
	struct nlp_value_t *v;
	for (v = head->next; v != head; v = head->next) {
		nlp_value_remove(v);
		nlp_value_free(v);
	}
}



/*
 * Purpose: dump value-list for debug
 * Arguments:
 *   struct nlp_value_t *head ... head of value-list to dump
 * Return value:
 *   (none)
 */
void nlp_value_dump(struct nlp_value_t *head)
{
	struct nlp_value_t *i;
	printf("BEGIN:NLP_VALUE_DUMP--------------------------------\n");
	for (i = head->next; i != head; i = i->next) {
		printf("(%8s:%s)\n", nlp_valtype_str[i->type], i->value);
	}
	printf("END:NLP_VALUE_DUMP----------------------------------\n");
}



/*
 * Purpose: initialize value-list
 * Arguments:
 *   (none)
 * Return value:
 *   (none)
 */
void nlp_value_init()
{
	memset(&nlp_value_head, 0, sizeof(struct nlp_value_t));
	nlp_value_head.prev = &nlp_value_head;
	nlp_value_head.next = &nlp_value_head;
}



/*
 * Purpose: 
 */
int nlp_value_append(int type, char *val)
{
	struct nlp_value_t *work_v;

	if (type < 0 || type > NLP_VALTYPE_MAX) return 1;
	if (type != NLP_VALTYPE_NULL && val == NULL) return 1;
	work_v = malloc(sizeof(struct nlp_value_t));
	if (work_v == NULL) return 1;
	if (val == NULL) {
		work_v->value = NULL;
	} else {
		work_v->value = strdup(val);
		if (work_v->value == NULL) {
			free(work_v);
			return 1;
		}
	}
	work_v->type = type;
	work_v->first_line = yylloc.first_line;
	work_v->last_line = yylloc.last_line;
	work_v->first_column = yylloc.first_column;
	work_v->last_column = yylloc.last_column;
	work_v->prev = nlp_value_head.prev;
	work_v->next = &nlp_value_head;
	nlp_value_head.prev->next = work_v;
	nlp_value_head.prev = work_v;
	return 0;
}



/*
 *
 */
void nlp_value_purge()
{
	struct nlp_value_t *v;
	struct nlp_value_t *free_v;

	v = nlp_value_head.next;
	if (v == &nlp_value_head) return;

	v->prev->next = v->next;
	v->next->prev = v->prev;
	if (v->value) free(v->value);
	free(v);

	v = nlp_value_head.next;
	while (v != &nlp_value_head) {
		if (v->type == NLP_VALTYPE_IDENTIFIER) break;
		v->prev->next = v->next;
		v->next->prev = v->prev;
		if (v->value) free(v->value);
		free_v = v;
		v = v->next;
		free(free_v);
	}
}



/*
 */
struct nlp_value_t *nlp_value_get_head()
{
	return &nlp_value_head;
}



int nlp_value_calc_index(struct nlp_variable_t *variable, int address[], int *index)
{
	int i;
	int size;
	int ind;

	ind = 0;
	size = 1;
	for (i = 0; i < variable->dimension; i++) {
		ind += (address[i] - variable->dimension_range_minimum[i]) * size;
		size *= variable->dimension_range_maximum[i] - variable->dimension_range_minimum[i] + 1;
	}
	*index = ind;
	for (i = 0; i < variable->dimension; i++) {
		if (address[i] < variable->dimension_range_minimum[i] ||
			address[i] > variable->dimension_range_maximum[i]) return 1;
	}

	return 0;
}



int nlp_value_increment_index(struct nlp_variable_t *variable, int address[], int *index)
{
	int i;

	(*index)++;
	address[0]++;
	for (i = 0; i < variable->dimension - 1; i++) {
		if (address[i] > variable->dimension_range_maximum[i]) {
			address[i + 1]++;
			address[i] = variable->dimension_range_minimum[i];
		}
	}
	if (address[variable->dimension - 1] > variable->dimension_range_maximum[variable->dimension - 1]) return 1;
	return 0;
}



int nlp_value_skip_dimension(struct nlp_variable_t *variable, int address[], int *index, char *skipper)
{
	int i;
	int l;
	int edge;

	l = (int)strlen(skipper);
	if (l >= variable->dimension) {
		for (i = 0; i < variable->dimension - 1; i++) {
			address[i] = variable->dimension_range_minimum[i];
		}
		address[variable->dimension - 1] = variable->dimension_range_maximum[variable->dimension - 1] + 1;

	} else {
		edge = 1;
		for (i = 0; i <= l - 1; i++) {
			if (address[i] != variable->dimension_range_minimum[i]) {
				edge = 0;
			}
		}
		for (i = 0; i <= l - 1; i++) {
			address[i] = variable->dimension_range_minimum[i];
		}
		if (edge == 0) {
			address[l]++;
			for (i = l; i < variable->dimension - 1; i++) {
				if (address[i] > variable->dimension_range_maximum[i]) {
					address[i + 1]++;
					address[i] = variable->dimension_range_minimum[i];
				}
			}
		}
	}
	nlp_value_calc_index(variable, address, index);
	return 0;
}


/*
 */
int nlp_value_let(struct nlp_variable_t *variable, int index, struct nlp_value_t *value)
{
	__int4 *ip;
	__int8 *lp;
	__real4 *fp;
	__real8 *dp;
	char *sp;
	char *p;
	__real8 tmp_r8;

	if (value->type == NLP_VALTYPE_NULL) return 0;
	switch (variable->type) {
	case NLP_VARTYPE_INTEGER:
		ip = (__int4 *)variable->p;
		switch (value->type) {
		case NLP_VALTYPE_INTEGER:
			ip[index] = strtol(value->value, &p, 10);
			if (*p != '\0') {
				//if ();
			}
			break;
		case NLP_VALTYPE_REAL:
			tmp_r8 = strtod(value->value, &p);
			if (*p != '\0') {
				//if ();
			}
			if ((variable->option & NLP_WRN_MASK) == NLP_WRN_REPORT) {
fprintf(stderr, "ip[%d] = %d (%s)\n", index, (int)atof(value->value), value->value);
			}
			if (tmp_r8 > INT_MAX) {
			}
			if (tmp_r8 < INT_MIN) {
			}
			ip[index] = (__int4)tmp_r8;
			break;
		default:
			/*FIXME ERROR*/
			break;
		}
		break;

	case NLP_VARTYPE_INTEGER8:
		lp = (__int8 *)variable->p;
		switch (value->type) {
		case NLP_VALTYPE_INTEGER:
			lp[index] = strtoll(value->value, &p, 10);
			if (*p != '\0') {
				//if ();
			}
			break;
		case NLP_VALTYPE_REAL:
			tmp_r8 = strtod(value->value, &p);
			if (*p != '\0') {
				//if ();
			}
			if ((variable->option & NLP_WRN_MASK) == NLP_WRN_REPORT) {
fprintf(stderr, "ip[%d] = %d (%s)\n", index, (int)atof(value->value), value->value);
			}
			if (tmp_r8 > LLONG_MAX) {
			}
			if (tmp_r8 < LLONG_MIN) {
			}
			lp[index] = (__int8)tmp_r8;
			break;
		default:
			break;
		}
		break;

	case NLP_VARTYPE_REAL:
		fp = (float *)variable->p;
		switch (value->type) {
		case NLP_VALTYPE_INTEGER:
		case NLP_VALTYPE_REAL:
			tmp_r8 = strtod(value->value, &p);
			if (*p != '\0') {
				//if ();
			}
			if ((variable->option & NLP_WRN_MASK) == NLP_WRN_REPORT) {

			}
			if (tmp_r8 > FLT_MAX) {

			}
			if (tmp_r8 < FLT_MIN) {

			}
			fp[index] = (__real4)tmp_r8;
			break;
		default:
			/*FIXME ERROR*/
			break;
		}
		break;

	case NLP_VARTYPE_REAL8:
		dp = (double *)variable->p;
		switch (value->type) {
		case NLP_VALTYPE_INTEGER:
		case NLP_VALTYPE_REAL:
			dp[index] = strtod(value->value, &p);
			if (*p != '\0') {

			}
			break;
		}
		break;

	case NLP_VARTYPE_FIXCHAR:
		sp = value->value;
		
		break;

	case NLP_VARTYPE_STRING:
		sp = value->value;
		break;
	}
	return 0;
}



/*
 * Purpose: initialize decode stack
 * Arguments:
 *   (none)
 * Return value:
 *   (none)
 */
void nlp_decode_init()
{
	memset(&nlp_decode_head, 0, sizeof(struct nlp_value_t));
	nlp_decode_head.prev = &nlp_decode_head;
	nlp_decode_head.next = &nlp_decode_head;
}



/*
 * Return value:
 *   = 0 ... success
 *   = 1 ... error
 *   int start_address[] ... start address
 */
int nlp_value_decode_get_start_address(struct nlp_variable_t *variable, int start_address[])
{
	int dim;
	int start_dim;
	int i;

	dim = nlp_get_addr_dimension();
	if (dim > NLP_MAX_DIMENSION) {
/*FIXME*/
	}
	if (dim == 0) {
		for (i = 0; i < variable->dimension; i++) {
			start_address[i] = variable->dimension_range_minimum[i];
		}
		return 0;
	}
	start_dim = 0;
	if (dim < variable->dimension) {
		start_dim = variable->dimension - dim;
		for (i = 0; i < start_dim; i++) {
			start_address[i] = variable->dimension_range_minimum[i];
		}
	}
	for (i = 0; i < dim; i++) {
		start_address[start_dim + i] = nlp_get_addr(i + 1);
	}
	return 0;
}



/*
 * Return Value:
 *   = 0 ... Success
 *   = 1 ... Variable not found
 *   = 2 ... 
 */
int nlp_value_decode()
{
	struct nlp_value_t *varname;
	struct nlp_variable_t *var;
	struct nlp_value_t *c;
	struct nlp_value_t *p;
	char buf[32];
	int multiply;
	int cdst[NLP_MAX_DIMENSION];
	int dst;
	int i;
	int j;

	varname = nlp_value_head.next;
	nlp_value_remove(varname);

	var = nlp_variable_search(varname->value);
	if (var == NULL) {
		nlp_log("ERROR: invalid variable found [%s]. in line no %d column %d\n",
			varname->value,
			varname->first_line,
			varname->first_column);
		return 1;
	}
	nlp_value_decode_get_start_address(var, cdst);
	nlp_value_calc_index(var, cdst, &dst);

	nlp_decode_head.next = nlp_value_head.next;
	nlp_decode_head.prev = nlp_value_head.prev;
	nlp_decode_head.next->prev = &nlp_decode_head;
	nlp_decode_head.prev->next = &nlp_decode_head;
	nlp_value_init();
	p = nlp_decode_head.prev;
	if (p->type == NLP_VALTYPE_IDENTIFIER) {
		nlp_value_remove(p);
		nlp_value_insert(&nlp_value_head, p);
	}
/*FIXME*/
	nlp_value_dump(&nlp_decode_head);
	nlp_decode_stack_pointer = -1;
	memset(nlp_decode_stack, 0, sizeof(nlp_decode_stack));
	multiply = 1;
	for (c = nlp_decode_head.next; c != &nlp_decode_head; c = c->next) {
		if (c->type == NLP_VALTYPE_MULTIPLY) {
			if (strcmp(c->value, "**") == 0) {
				multiply = -1;
			} else {
				t_strlcpy(buf, c->value, sizeof(buf));
				i = (int)strlen(buf);
				buf[i - 1] = '\0';
				multiply = atoi(buf);
			}
		} else if (c->type == NLP_VALTYPE_LP) {
			nlp_decode_stack_pointer++;
			nlp_decode_stack[nlp_decode_stack_pointer].head = c->next;
			nlp_decode_stack[nlp_decode_stack_pointer].multiply = multiply;
			multiply = 1;
		} else if (c->type == NLP_VALTYPE_RP) {
			if (nlp_decode_stack[nlp_decode_stack_pointer].multiply == -1) {
				c = nlp_decode_stack[nlp_decode_stack_pointer].head->prev;
				continue;
			} else {
				nlp_decode_stack[nlp_decode_stack_pointer].multiply--;
				if (nlp_decode_stack[nlp_decode_stack_pointer].multiply > 0) {
					c = nlp_decode_stack[nlp_decode_stack_pointer].head->prev;
					continue;
				}
				nlp_decode_stack_pointer--;
			}
		} else if (c->type == NLP_VALTYPE_SKIPPER) {
			nlp_value_skip_dimension(var, cdst, &dst, c->value);

		} else {
			if (multiply > 0) {
				for (i = 0; i < multiply; i++) {
/*FIXME*/
					printf("%5d: ", dst);
					for (j = 0; j < var->dimension; j++) {
						printf("%3d ", cdst[j]);
					}
					printf(": %20s: %s\n", c->value, nlp_valtype_str[c->type]);
					nlp_value_let(var, dst, c);
					nlp_value_increment_index(var, cdst, &dst);
				}
				multiply = 1;
			} else {
				for (; dst < var->array_size;) {
/*FIXME*/
					printf("%5d: ", dst);
					for (j = 0; j < var->dimension; j++) {
						printf("%3d ", cdst[j]);
					}
					printf(": %20s: %s\n", c->value, nlp_valtype_str[c->type]);
					nlp_value_let(var, dst, c);
					nlp_value_increment_index(var, cdst, &dst);
				}
			}
		}

	}
	nlp_value_list_free(&nlp_decode_head);
	return 0;
}
