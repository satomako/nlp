/* nlp_value.h
 * 2017-04-30
 */
#ifndef   _NLP_VALUE_H_
#define   _NLP_VALUE_H_

#include "nlp_variable.h"

#define NLP_VALTYPE_NULL       (0)
#define NLP_VALTYPE_IDENTIFIER (1)
#define NLP_VALTYPE_INTEGER    (2)
#define NLP_VALTYPE_REAL       (3)
#define NLP_VALTYPE_STRING     (4)
#define NLP_VALTYPE_MULTIPLY   (5)
#define NLP_VALTYPE_LP         (6)
#define NLP_VALTYPE_RP         (7)
#define NLP_VALTYPE_SKIPPER    (8)
#define NLP_VALTYPE_MAX        (8)

#define NLP_VALUE_NEST_MAX    (32)

struct nlp_filename_list_t
{
  struct nlp_filename_list_t *prev;
  struct nlp_filename_list_t *next;
  char *filename; 
};

struct nlp_value_t {
  struct nlp_value_t *prev;
  struct nlp_value_t *next;
  int type;
  char *filename;
  int first_line;
  int last_line;
  int first_column;
  int last_column;
  char *value;
};

struct nlp_decode_stack_t {
  int multiply;
  struct nlp_value_t *head;
};

void nlp_filename_list_init();
char *nlp_filename_list_add(char *filename);
int nlp_filename_list_clear();
void nlp_value_set_logfile(FILE *logfile);
void nlp_value_insert(struct nlp_value_t *dst, struct nlp_value_t *v);
void nlp_value_remove(struct nlp_value_t *v);
void nlp_value_free(struct nlp_value_t *v);
void nlp_value_list_free(struct nlp_value_t *head);
void nlp_value_dump(struct nlp_value_t *head);
void nlp_value_init();
void nlp_decode_init();
int nlp_value_append(int type, char *val);
void nlp_value_purge();
struct nlp_value_t *nlp_value_get_head();
int nlp_value_decode_get_start_address(struct nlp_variable_t *variable, int start_address[]);
int nlp_value_calc_index(struct nlp_variable_t *variable, int address[], int *index);
int nlp_value_increment_index(struct nlp_variable_t *variable, int address[], int *index);
int nlp_value_check_address(struct nlp_variable_t *variable, int address[]);
int nlp_value_skip_dimension(struct nlp_variable_t *variable, int address[], int *index, char *skipper);
int nlp_value_decode();

#endif /* _NLP_VALUE_H_ */
