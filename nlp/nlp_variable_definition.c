#include <stdio.h>
#include <stdlib.h>
typedef void *yyscan_t;
#define YY_NO_UNISTD_H
#include "nlp.h"
#include "nlp_internal.tab.h"
union YYSTYPE
{
    char *str;
};
typedef union YYSTYPE YYSTYPE;
#include "nlp_internal.lex.h"

static int nlp_concrete_decode_variable_definition_dimension(struct nlp_value_t **val, struct nlp_variable_t *var);

int nlp_init_variable_definition(struct nlp_t *c)
{
    zzlex_init(&c->internal_scanner);
    if (c->internal_scanner == NULL)
    {
        return NLP_ERR_BAD_STATUS;
    }
    zzset_extra(c, c->internal_scanner);
    return NLP_NOERR;
}

int nlp_add_variable_definition_token(struct nlp_t *c, int type, char *value, struct ZZLTYPE **yyl)
{
    struct nlp_value_t *i;
    struct nlp_value_t *v;

    if (c == NULL)
    {
        return NLP_ERR_BAD_ARGUMENT;
    }
    for (i = &c->working_variable_definition_head; i; i = i->next)
    {
        if (i->next == NULL)
            break;
    }

    v = calloc(1, sizeof(struct nlp_value_t));
    if (v == NULL)
    {
        return NLP_ERR_BAD_STATUS;
    }

    v->value = t_strdup(value);
    if (v->value == NULL)
        return NLP_ERR_BAD_STATUS;
    v->type = type;
    v->first_line = (*yyl)->first_line;
    v->first_column = (*yyl)->first_column;
    i->next = v;
    return NLP_NOERR;
}

int _nlp_dump_variable_definition(struct nlp_t *c)
{
    struct nlp_value_t *i;
    for (i = c->working_variable_definition_head.next; i; i = i->next)
    {
        printf("type=%d, value=%s\n", i->type, i->value);
    }
    return NLP_NOERR;
}

int nlp_concrete_decode_variable_definition(struct nlp_t *c)
{
    struct nlp_variable_t *v;
    struct nlp_value_t *i;

    for (i = c->working_variable_definition_head.next; i; i = i->next)
    {
        switch (i->type)
        {
        case TYPEDEF:
            v = calloc(1, sizeof(struct nlp_variable_t));
            if (v == NULL)
            {
                return NLP_ERR_BAD_STATUS;
            }
            break;
        case INT8:
        case INT32:
        case INT64:
        case REAL32:
        case REAL64:
        case STRING:
            v = calloc(1, sizeof(struct nlp_variable_t));
            if (v == NULL)
            {
                return NLP_ERR_BAD_STATUS;
            }
            if (i->type == INT8)
            {
                v->type = NLP_TYPE_INT8;
                v->size = 1;
            }
            if (i->type == INT32)
            {
                v->type = NLP_TYPE_INT32;
                v->size = 4;
            }
            if (i->type == INT64)
            {
                v->type = NLP_TYPE_INT64;
                v->size = 8;
            }
            if (i->type == REAL32)
            {
                v->type = NLP_TYPE_REAL32;
                v->size = 4;
            }
            if (i->type == REAL64)
            {
                v->type = NLP_TYPE_REAL64;
                v->size = 8;
            }
            if (i->type == STRING)
            {
                v->type = NLP_TYPE_FSTRING;
            }

            i = i->next;
            t_strlcpy(v->name, i->value, sizeof(v->name));

            i = i->next;
            nlp_concrete_decode_variable_definition_dimension(&i, v);
            break;
        }
    }
    return NLP_NOERR;
}

static int nlp_concrete_decode_variable_definition_dimension(struct nlp_value_t **val, struct nlp_variable_t *var)
{
    int min;
    int max;
    int range;

    min = 1;
    range = 0;
    for (; (*val)->type != SM; *val = (*val)->next)
    {
        if ((*val)->type == LP)
            continue;
        if ((*val)->type == RP || (*val)->type == CM)
        {
            if (var->dim >= NLP_MAX_DIMENSION)
            {
                return NLP_ERR_BAD_STATUS;
            }
            var->min[var->dim] = min;
            var->max[var->dim] = max;
            var->dim++;
            range = 0;
            min = 1;
        }
        if ((*val)->type == INTEGER)
        {
            if (range == 0)
            {
                max = strtol((*val)->value, NULL, 10);
            }
            else
            {
                min = max;
                max = strtol((*val)->value, NULL, 10);
            }
        }
        if ((*val)->type == COLON)
        {
            range = 1;
        }
    }
    return NLP_NOERR;
}