#ifndef __NLP_H__
#define __NLP_H__

#define NLP_MAX_IDENTIFIER_LEN   (127)
#define NLP_MAX_DIMENSION        (8)

#define NLP_NOERR                (0)
#define NLP_ERR_NULL_POINTER     (1)
#define NLP_ERR_NOMEM            (2)
#define NLP_ERR_VARIABLE_EXISTS  (3)
#define NLP_ERR_MEMBER_EXISTS    (4)

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


struct nlp_variable_t
{
    char name[NLP_MAX_IDENTIFIER_LEN+1];
    int type;
    int size;
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

struct nlp_vlalue_t
{
    char *value;
    int type;
    // TODO add positions
};

struct nlp_value_list_t
{
    struct nlp_value_list_t *next;
    struct nlp_value_t *value;
};

struct nlp_decode_variable_t
{
    struct nlp_variable_t *variable;
    struct nlp_value_list_t *value_list;
    int value_sp;
    struct nlp_value_list_t *value_stack[128];
    int variable_sp;
    struct nlp_variable_t *variable_stack[128];
    struct nlp_variable_t *current;
    void *p;
};

struct nlp_t
{
    struct nlp_variable_list_t variable_list_head;
    struct nlp_decode_variable_t decode_variable;
};

struct nlp_variable_t *nlp_create_variable(char *name, int type, int size, int dim, int min[], int max[]);
int nlp_dispose_variable(struct nlp_variable_t *variable);
int nlp_add_member(struct nlp_variable_t *variable, struct nlp_variable_t *member);
struct nlp_variable_t *nlp_find_member(struct nlp_variable_t *variable, char *name);
struct nlp_variable_t *nlp_clone_variable(struct nlp_variable_t *variable);
int nlp_calc_struct_size(struct nlp_variable_t *variable);
int nlp_add_variable(struct nlp_variable_list_t *variable_list, struct nlp_variable_t *variable);
struct nlp_variable_t *nlp_find_variable(struct nlp_variable_list_t *variable_list, char *name);
int nlp_append_variable_list(struct nlp_variable_list_t *variable_list, struct nlp_variable_t *variable);

int nlp_forward_reference(struct nlp_decode_variable_t *v);

char *t_strlcpy(char *dst, char *src, int size);

#endif