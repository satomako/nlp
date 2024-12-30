#ifndef __NLP_H__
#define __NLP_H__

#define NLP_MAX_IDENTIFIER_LEN   (127)
#define NLP_MAX_DIMENSION        (8)
#define NLP_MAX_MEMBER_DEPTH     (32)
#define NLP_MAX_VALUE_NEST       (32)
#define NLP_MAX_STRING_LENGTH    (65536)
#define NLP_BUFFER_SIZE          (1024)

#define NLP_NOERR                (0)
#define NLP_ERR_NULL_POINTER     (1)
#define NLP_ERR_NOMEM            (2)
#define NLP_ERR_VARIABLE_EXISTS  (3)
#define NLP_ERR_MEMBER_EXISTS    (4)
#define NLP_ERR_BAD_ARGUMENT     (5)
#define NLP_ERR_BAD_STATUS       (6)

#define NLP_TYPE_INT8            (1)
#define NLP_TYPE_INT32           (2)
#define NLP_TYPE_INT64           (3)
#define NLP_TYPE_REAL32          (4)
#define NLP_TYPE_REAL64          (5)
#define NLP_TYPE_FSTRING         (6)
#define NLP_TYPE_STRUCT          (7)

#define NLP_STRING_TOUPPER       (1<<0)
#define NLP_STRING_TOLOWER       (1<<1)
#define NLP_STRING_PAD_SPACE     (1<<2)

#define NLP_VALUE_STATE_INIT       (0)
#define NLP_VALUE_STATE_IDENTIFIER (1)

#define NLP_DECODE_STATE_VALUE   (0)
#define NLP_DECODE_STATE_COMMA   (1)

#define NLP_FALSE                (0)
#define NLP_TRUE                 (1)

#if ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
};
#define YYLTYPE_IS_DECLARED 1
#endif
int yylex();
void yyerror(const char *);
int yywrap();
void yyerror(const char *str);


struct nlp_variable_t
{
    char name[NLP_MAX_IDENTIFIER_LEN+1];
    int type;
    int size;
    int padding;
    int msize;
    int dim;
    int min[NLP_MAX_DIMENSION];
    int max[NLP_MAX_DIMENSION];
    int cur[NLP_MAX_DIMENSION];
    struct nlp_variable_t* next;
    struct nlp_variable_t* member;
    int level;
};

struct nlp_variable_list_t
{
    struct nlp_variable_list_t *prev;
    struct nlp_variable_list_t *next;
    struct nlp_variable_t *v;
};

struct nlp_value_t
{
    struct nlp_value_t *next;
    char *value;
    int type;
    int first_line;
    int first_column;
    int last_line;
    int last_column;
};


struct nlp_value_list_t
{
    struct nlp_value_list_t *next;
    struct nlp_value_t list_head;
};

struct nlp_decode_variable_t
{
    struct nlp_variable_t *variable;
    struct nlp_value_t *value_list;
    int value_sp;
    struct nlp_value_t *value_stack[NLP_MAX_VALUE_NEST];
    int value_multiply[NLP_MAX_VALUE_NEST];
    int comma_state[NLP_MAX_VALUE_NEST];
    int variable_sp;
    struct nlp_variable_t *variable_stack[NLP_MAX_MEMBER_DEPTH];
    struct nlp_variable_t *current;
    int exceed;
    int decode_state;
    void *p;
};

struct nlp_t
{
    struct nlp_variable_list_t variable_list_head;
    struct nlp_decode_variable_t decode_variable;
    struct nlp_value_list_t value_list_head;
    struct nlp_value_t *current_value_list_head;
    int value_state;
    int opt_dryrun;
    int opt_verbose;
    char scanning_string[NLP_MAX_STRING_LENGTH+1];
    int scanning_string_length;
};

/*
 * nlp.c
*/
int nlp_init(struct nlp_t *c);
int nlp_set_dryrun(struct nlp_t *c, int option);
struct nlp_t *nlp_get_current_context();
int nlp_decode(struct nlp_t *c);
int nlp_decode_one_variable(struct nlp_t *c, struct nlp_variable_t *var, struct nlp_value_list_t *vl);
int nlp_decode_set_start_address(struct nlp_t *c, struct nlp_variable_t *var, struct nlp_value_t **val);
int nlp_decode_dimension(struct nlp_decode_variable_t *dc, struct nlp_value_t **val);
int nlp_decode_values(struct nlp_decode_variable_t *dc, struct nlp_value_t *val);

/* 
 * nlp_variable.c
*/
struct nlp_variable_t *nlp_create_variable(char *name, int type, int size, int padding, int dim, int min[], int max[]);
int nlp_dispose_variable(struct nlp_variable_t *variable);
int nlp_add_member(struct nlp_variable_t *variable, struct nlp_variable_t *member);
struct nlp_variable_t *nlp_find_member(struct nlp_variable_t *variable, char *name);
struct nlp_variable_t *nlp_clone_variable(struct nlp_variable_t *variable);
int nlp_calc_struct_size(struct nlp_variable_t *variable, struct nlp_variable_t *stack[], int sp);
int nlp_add_variable(struct nlp_variable_list_t *variable_list, struct nlp_variable_t *variable);
struct nlp_variable_t *nlp_find_variable(struct nlp_variable_list_t *variable_list, char *name);
int nlp_append_variable_list(struct nlp_variable_list_t *variable_list, struct nlp_variable_t *variable);
int nlp_forward_reference(struct nlp_decode_variable_t *v);

/*
 * nlp_value.c
*/
int nlp_value_list_count(struct nlp_t *c);
struct nlp_value_list_t *nlp_create_value_list();
int nlp_add_value_list(struct nlp_t *c);
void nlp_remove_value_list_first(struct nlp_t *c);
void nlp_free_value_list(struct nlp_t *c);
void nlp_free_values(struct nlp_value_t *l);
int nlp_add_value(struct nlp_t *c, int vtype, char *value, struct YYLTYPE *yyl);
void nlp_string_reset(struct nlp_t *c);
int nlp_string_append(struct nlp_t *c, char chr);

/*debug*/
void _nlp_dump_values(struct nlp_t *c);
void _nlp_print_values(struct nlp_value_t *l);
void _nlp_print_value_list(struct nlp_t *c);

/*
 * nlp_util.c
*/
char *t_strlcpy(char *dst, char *src, int size);
char *t_strdup(char *src);

#endif