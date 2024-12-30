#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlp.h"
#include "nlp.tab.h"

//int _nlp_print_current(struct nlp_t *c);
int _nlp_print_current(struct nlp_decode_variable_t *dc, int opt);
char* _nlp_type_to_string(int type);

/*
 * current context
*/
static struct nlp_t *_cc;

void yyerror(const char *);

int yywrap()
{
	return 1;
}

void yyerror(const char *str)
{
}

int nlp_init(struct nlp_t *c)
{
    memset(c, 0, sizeof(struct nlp_t));
    c->variable_list_head.next = &c->variable_list_head;
    c->variable_list_head.prev = &c->variable_list_head;
    c->opt_dryrun = 0;
    c->value_state = NLP_VALUE_STATE_INIT;

    //TODO
    _cc = c;
}

int nlp_set_dryrun(struct nlp_t *c, int option)
{
    if (option < 0 || option > 1) return NLP_ERR_BAD_ARGUMENT;
    c->opt_dryrun = option;
    return NLP_NOERR;
}

struct nlp_t *nlp_get_current_context()
{
    return _cc;
}

void nlp_set_current_context(struct nlp_t *c)
{
    _cc = c;
}

int nlp_decode(struct nlp_t *c)
{
    struct nlp_value_list_t *vl;
    struct nlp_variable_t *var;

    if (c == NULL) return NLP_ERR_BAD_ARGUMENT;
    for (vl = c->value_list_head.next; vl != NULL; vl = vl->next)    
    {
        fprintf(stderr, "*** nlp_decode value list p %p\n", vl);
        fprintf(stderr, "identifier=%s\n", vl->list_head.next->value);
        var = nlp_find_variable(&c->variable_list_head, vl->list_head.next->value);
        if (var == NULL)
        {
            fprintf(stderr, "variable not found\n");
        }
        else
        {
            fprintf(stderr, "variable found [ %s ]\n", var->name);
            nlp_decode_one_variable(c, var, vl);
        }
    }
    return NLP_NOERR;
}

int nlp_decode_one_variable(struct nlp_t *c, struct nlp_variable_t *variable, struct nlp_value_list_t *vl)
{
    struct nlp_decode_variable_t *dc;
    struct nlp_variable_t *var;
    struct nlp_value_t *val;
    int i;

//TODO
//fprintf(stderr, "nlp_decode_one_variable\n");
//fprintf(stderr, "*** 1 %s\n", variable->name);

    dc = &c->decode_variable;

    var = nlp_clone_variable(variable);
//TODO
//fprintf(stderr, "*** 2\n");
    if (var == NULL) return NLP_ERR_NOMEM;
    dc->variable = var;
    dc->variable_sp = 0;
    dc->variable_stack[dc->variable_sp] = var;
    for (i = 0; i < NLP_MAX_DIMENSION; i++)
    {
        dc->variable_stack[dc->variable_sp]->cur[i] = var->min[i];
    }
    dc->current = var;

    val = vl->list_head.next->next;
//TODO
//fprintf(stderr, "*** 3 (%p) %s\n", val, val->value);
    nlp_decode_set_start_address(c, variable, &val);
//TODO
//fprintf(stderr, "*** 4 _nlp_print_current\n");
//_nlp_print_current(c);
    nlp_decode_values(dc, val->next);

    return 0;
}


int nlp_decode_set_start_address(struct nlp_t *c, struct nlp_variable_t *variable, struct nlp_value_t **val)
{
    struct nlp_decode_variable_t *dc;
    struct nlp_variable_t *member;

    dc = &c->decode_variable;

    for (; *val; *val = (*val)->next)
    {
//TODO
//fprintf(stderr, "addr %s\n", (*val)->value);
        if ((*val)->type == LP)
        {
            nlp_decode_dimension(dc, val);
        }
        else if ((*val)->type == DOT)
        {
            *val = (*val)->next;
            member = nlp_find_member(dc->current, (*val)->value);
            if (member)
            {
                dc->variable_sp++;
                dc->variable_stack[dc->variable_sp] = member;
                dc->current = member;
//TODO
//fprintf(stderr, "member %s\n", member->name);
            }
            else
            {
                //TODO error handling
            }
        }
        if ((*val)->type == EQ)
        {
            break;
        }
    }
    return NLP_NOERR;
}


int nlp_decode_dimension(struct nlp_decode_variable_t *dc, struct nlp_value_t **val)
{
    int dim;
    long ind;
    char *endptr;

    if ((*val)->next == NULL) return NLP_ERR_BAD_STATUS;
    dim = 0;
    for (*val = (*val)->next; *val; *val = (*val)->next)
    {
        if ((*val)->type == RP)
        {
            break;
        }
        if ((*val)->type == CM)
        {
            dim++;
            continue;
        }
        if ((*val)->type == INTEGER)
        {
            ind = strtol((*val)->value, &endptr, 10);
            if (endptr)
            {
                //TODO error handling
            }
            dc->current->cur[dim] = (int)ind;
//fprintf(stderr, "dim %d %d\n", dim, (int)ind);
        }
    }
    return NLP_NOERR;
}


int nlp_decode_values(struct nlp_decode_variable_t *dc, struct nlp_value_t *val)
{
    int multiply;
    char buf[NLP_BUFFER_SIZE];
    size_t vlen;

//TODO
//    fprintf(stderr, "ENTER nlp_decode_values %s\n", dc->variable->name);
/*
    for (val = val->next; val; val = val->next)
    {
printf("value = %s\n", val->value);
_nlp_print_current(_cc);
fprintf(stderr, "value = %s\n", val->value);
    }
*/
    dc->value_sp = 0;
    dc->value_stack[dc->value_sp] = val;
    dc->comma_state[dc->value_sp] = 1;
    dc->value_multiply[dc->value_sp] = 1;
    dc->exceed = NLP_FALSE;
    dc->decode_state = NLP_DECODE_STATE_COMMA;

    multiply = 1;
    while (1)
    {
        vlen = strlen(val->value);
        if (val->type == MULMUL)
        {
//printf("XXX MULMUL\n");
            multiply = -1;
        }
        else if (val->type == NUMMUL)
        {
//printf("XXX NUMMUL\n");
            if (vlen >= NLP_BUFFER_SIZE)
            {
                //TODO
                return NLP_ERR_BAD_STATUS;
            }
            t_strlcpy(buf, val->value, sizeof(buf));
            buf[vlen - 1] = '\0';
            errno = 0;
            multiply = strtol(buf, NULL, 10);
            if (errno != 0)
            {
                return NLP_ERR_BAD_STATUS;
            }
        }
        else if (val->type == LP)
        {
//printf("XXX LP\n");
            dc->value_sp++;
            dc->value_stack[dc->value_sp] = val->next;
            dc->value_multiply[dc->value_sp] = multiply;
        }
        else if (val->type == RP)
        {
//printf("XXX RP\n");
            if (dc->value_multiply[dc->value_sp] == -1)
            {
                val = dc->value_stack[dc->value_sp];
            }
            else
            {
                dc->value_multiply[dc->value_sp]--;
                if (dc->value_multiply[dc->value_sp] > 0)
                {
                    val = dc->value_stack[dc->value_sp];
                }
                dc->value_sp--;
            }
        }
        else
        {
//printf("XXX ELSE\n");
            //if (dc->exceed)
            //{
            //}
//printf("value_sp       = %d\n", dc->value_sp);
//printf("value_multiply = %d\n", dc->value_multiply[dc->value_sp]);
//printf("multiply       = %d\n", multiply);
//printf("state          = %d\n", dc->decode_state);
//printf("value          = %s\n", val->value);

            if (multiply == -1)
            {
//printf("MULMUL %s\n", val->value);
                multiply = 1;
                break;
            }
            else
            {
                while (multiply > 0)
                {
//printf("while multiply = %d\n", dc->value_multiply[dc->value_sp]);
                    if (val->type == CM)
                    {
                        if (dc->decode_state == NLP_DECODE_STATE_COMMA)
                        {
//printf("addr++\n");
                            nlp_forward_reference(dc);
                        }
                        else
                        {
//printf("nothing\n");
                        }
                        dc->decode_state = NLP_DECODE_STATE_COMMA;
                    }
                    else
                    {
//printf("let addr++\n");
//_nlp_print_current(dc, 0);
//printf(" = %s\n", val->value);
_nlp_print_current(dc, 2);
printf(" = %s\n", val->value);
                        dc->decode_state = NLP_DECODE_STATE_VALUE;
                        nlp_forward_reference(dc);
                    }
                    multiply--;
                }
                multiply = 1;
            }
        }
        if (val->next == NULL) break;
        val = val->next;
    }
    return NLP_NOERR;
}


//int _nlp_print_current(struct nlp_t *c)
int _nlp_print_current(struct nlp_decode_variable_t *dc, int opt)
{
//    struct nlp_decode_variable_t *dc;
    int sp;

//    dc = &c->decode_variable;
    for (sp = 0; sp <= dc->variable_sp; sp++)
    {
        if (sp > 0)
        {
            printf(".");
        }
        printf("%s", dc->variable_stack[sp]->name);
        if (dc->variable_stack[sp]->dim > 0)
        {
            printf("(");
            for (int i = 0; i < dc->variable_stack[sp]->dim; i++)
            {
                if (i > 0)
                {
                    printf(",");
                }
                printf("%d", dc->variable_stack[sp]->cur[i]);
            }
            printf(")");
        }
    }
    if (opt & 2)
    {
        printf("\n    type: %s\n", _nlp_type_to_string(dc->current->type));
        printf("    ");
    }
    if (opt & 1) printf("\n");
    return NLP_NOERR;
}

char* _nlp_type_to_string(int type)
{
    if (type == NLP_TYPE_INT8) return "int8";
    if (type == NLP_TYPE_INT32) return "int32";
    if (type == NLP_TYPE_INT64) return "int64";
    if (type == NLP_TYPE_REAL32) return "real32";
    if (type == NLP_TYPE_REAL32) return "real32";
    if (type == NLP_TYPE_FSTRING) return "fixed string";
    return "(unknown)";
}