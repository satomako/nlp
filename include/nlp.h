#ifndef __NLP_H__
#define __NLP_H__

#define NLP_MAX_IDENTIFIER_LEN (127)
#define NLP_MAX_DIMENSION (8)
#define NLP_MAX_MEMBER_DEPTH (32)
#define NLP_MAX_VALUE_NEST (32)
#define NLP_MAX_STRING_LENGTH (65536)
#define NLP_BUFFER_SIZE (1024)

#define NLP_NOERR (0)               // no error
#define NLP_ERR_NULL_POINTER (1)    // null pointer
#define NLP_ERR_NOMEM (2)           // no memory
#define NLP_ERR_VARIABLE_EXISTS (3) // variable exists
#define NLP_ERR_MEMBER_EXISTS (4)   // member exists
#define NLP_ERR_BAD_ARGUMENT (5)    // bad argument
#define NLP_ERR_BAD_STATUS (6)      // bad status

#define NLP_TYPE_INT8 (1)
#define NLP_TYPE_INT32 (2)
#define NLP_TYPE_INT64 (3)
#define NLP_TYPE_REAL32 (4)
#define NLP_TYPE_REAL64 (5)
#define NLP_TYPE_FSTRING (6)
#define NLP_TYPE_STRUCT (7)

#define NLP_STRING_TOUPPER (1 << 0)
#define NLP_STRING_TOLOWER (1 << 1)
#define NLP_STRING_PAD_SPACE (1 << 2)

#define NLP_VALUE_STATE_INIT (0)
#define NLP_VALUE_STATE_IDENTIFIER (1)

#define NLP_DECODE_STATE_VALUE (0)
#define NLP_DECODE_STATE_COMMA (1)

#define NLP_FALSE (0)
#define NLP_TRUE (1)

#if !defined YYLTYPE_IS_DECLARED
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

#if !defined ZZLTYPE_IS_DECLARED
typedef struct ZZLTYPE ZZLTYPE;
struct ZZLTYPE
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
};
#define ZZLTYPE_IS_DECLARED 1
#endif

/**
 * @brief nlp variable structure
 * @param name variable name
 * @param struct_name struct name
 * @param type variable type
 * @param size variable size
 * @param padding padding size
 * @param msize member size
 * @param dim dimension
 * @param min minimum value of array subscript
 * @param max maximum value of array subscript
 * @param cur current value of array subscript
 * @param next next variable
 * @param member member
 * @param level level of member nesting
 */
struct nlp_variable_t
{
    char name[NLP_MAX_IDENTIFIER_LEN + 1];
    char struct_name[NLP_MAX_IDENTIFIER_LEN + 1];
    int type;
    int size;
    int padding;
    int msize;
    int dim;
    int min[NLP_MAX_DIMENSION];
    int max[NLP_MAX_DIMENSION];
    int cur[NLP_MAX_DIMENSION];
    struct nlp_variable_t *next;
    struct nlp_variable_t *member;
    int level;
};

/**
 * @brief nlp variable list structure
 * @param prev previous variable list
 * @param next next variable list
 * @param v variable
 */
struct nlp_variable_list_t
{
    struct nlp_variable_list_t *prev;
    struct nlp_variable_list_t *next;
    struct nlp_variable_t *v;
};

/**
 * @brief nlp value structure
 * @param next next value
 * @param value value
 * @param type value type
 * @param first_line first line in input
 * @param first_column first column in input
 * @param last_line last line in input
 * @param last_column last column in input
 */
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

/**
 * @brief nlp value list structure
 * @param next next value list
 * @param list_head list head
 */
struct nlp_value_list_t
{
    struct nlp_value_list_t *next;
    struct nlp_value_t list_head;
};

/**
 * @brief nlp decode variable structure
 * @param variable variable
 * @param value_list value list
 * @param value_sp value stack pointer
 * @param value_stack value stack
 * @param value_multiply value multiply
 * @param comma_state comma state
 * @param variable_sp variable stack pointer
 * @param variable_stack variable stack
 * @param current current variable
 * @param exceed exceed flag
 * @param decode_state decode state
 * @param p pointer of real variable
 */
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

/**
 * @brief nlp context structure
 * @param variable_list_head variable list head
 * @param decode_variable decode variable
 * @param value_list_head value list head
 * @param current_value_list_head current value list head
 * @param value_state value state
 * @param opt_dryrun dryrun option
 * @param opt_verbose verbose option
 * @param scanning_string scanning string
 * @param scanning_string_length scanning string length
 */
struct nlp_t
{
    yyscan_t scanner;
    yyscan_t internal_scanner;
    struct nlp_variable_list_t struct_list_head;
    struct nlp_variable_list_t variable_list_head;
    struct nlp_decode_variable_t decode_variable;
    struct nlp_value_list_t value_list_head;
    struct nlp_value_t *current_value_head;
    struct nlp_value_t working_variable_definition_head;
    int value_state;
    int opt_dryrun;
    int opt_verbose;
    char scanning_string[NLP_MAX_STRING_LENGTH + 1];
    int scanning_string_length;
};

/*
 * nlp.c
 */
int nlp_init(struct nlp_t *c);
int nlp_set_dryrun(struct nlp_t *c, int option);
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
int nlp_decode_variable_definition_file(struct nlp_t *c, char *filename);
int nlp_decode_variable_definition(struct nlp_t *c, char *input);

/*
 * nlp_value.c
 */
int nlp_value_list_count(struct nlp_t *c);
struct nlp_value_list_t *nlp_create_value_list();
int nlp_add_value_list(struct nlp_t *c);
void nlp_remove_value_list_first(struct nlp_t *c);
void nlp_free_value_list(struct nlp_t *c);
void nlp_free_values(struct nlp_value_t *l);
int nlp_add_value(struct nlp_t *c, int vtype, char *value, struct YYLTYPE **yyl);
void nlp_string_reset(struct nlp_t *c);
int nlp_string_append(struct nlp_t *c, char chr);

/*debug*/
void _nlp_dump_values(struct nlp_t *c);
void _nlp_print_values(struct nlp_value_t *l);
void _nlp_print_value_list(struct nlp_t *c);

/*
 * nlp_variable_definition.c
 */
int nlp_init_variable_definition(struct nlp_t *c);
int nlp_add_variable_definition_token(struct nlp_t *c, int type, char *value, struct ZZLTYPE **yyl);
// int nlp_add_variable_definition_token(struct nlp_t *c, int type, char *value, void *yyl);
int _nlp_dump_variable_definition(struct nlp_t *c);
int nlp_concrete_decode_variable_definition(struct nlp_t *c);

/*
 * nlp_util.c
 */
char *t_strlcpy(char *dst, char *src, int size);
char *t_strdup(char *src);
char *t_strlcpy_fix(char *dst, char *src, int size, int opt);
size_t f_filesize(char *filename);
int f_readfile(char *filename, char *buffer, size_t size);

#endif