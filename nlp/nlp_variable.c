#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlp.h"
#define MAX_NEST (128)

/**
 * @brief create new variable structure
 * @param[in] name variable name
 * @param[in] type variable type (NLP_TYPE_INT8/NLP_TYPE_INT32/)
 * @param[in] size variable size
 * @param[in] dim variable dimension
 * @param[in] min 
 * @param[in] max 
 * @return pointer of created variable, NULL at failed
*/
struct nlp_variable_t * nlp_create_variable(char *name, int type, int size, int dim, int min[], int max[])
{
    struct nlp_variable_t *v;

    if (name == NULL) return NULL;
    if (strlen(name) > NLP_MAX_IDENTIFIER_LEN) return NULL;
    if (type < 1 || type > NLP_TYPE_STRUCT) return NULL;
    if (type == NLP_TYPE_FSTRING && size < 1) return NULL;
    if (dim < 0 || dim > NLP_MAX_DIMENSION) return NULL;
    if (min == NULL) return NULL;
    if (max == NULL) return NULL;
    for (int i = 0; i < dim; i++)
    {
        if (min[i] > max[i]) return NULL;
    }

    if ((v = calloc(1, sizeof(struct nlp_variable_t))) == NULL) return NULL;
    t_strlcpy(v->name, name, sizeof(v->name));
    v->type = type;
    switch (type)
    {
        case NLP_TYPE_INT8:
            v->size = 1;
            break;
        case NLP_TYPE_INT32:
        case NLP_TYPE_REAL32:
            v->size = 4;
            break;
        case NLP_TYPE_INT64:
        case NLP_TYPE_REAL64:
            v->size = 8;
            break;
        case NLP_TYPE_STRUCT:
            v->size = size;
            break;
        default:
            v->size = 0;
    }
    v->dim = dim;
    for (int i = 0; i < dim; i++)
    {
        v->min[i] = min[i];
        v->max[i] = max[i];
    }
    return v;
}

/**
 * @brief
*/
int nlp_add_member(struct nlp_variable_t *variable, struct nlp_variable_t *member)
{
    struct nlp_variable_t *v;

    if (variable == NULL) return NLP_ERR_NULL_POINTER;
    if (member == NULL) return NLP_ERR_NULL_POINTER;
    if (variable->member == NULL)
    {
        variable->member = member;
    }
    else
    {
        if (nlp_find_member(variable, member->name) != NULL) return NLP_ERR_MEMBER_EXISTS;

        for (v = variable->member; v != NULL; v = v->next)
        {
            if (v->next == NULL)
            {
                v->next = member;
                break;
            }
        }
    }

    member->level = variable->level + 1;
    return NLP_NOERR;
}

/**
 * @brief
*/
struct nlp_variable_t * nlp_find_member(struct nlp_variable_t *variable, char *name)
{
    struct nlp_variable_t *v;

    if (variable == NULL) return NULL;
    if (name == NULL) return NULL;
    if (variable->type != NLP_TYPE_STRUCT) return NULL;

    for (v = variable->member; v != NULL; v = v->next)
    {
        if (strcmp(v->name, name) == 0) return v;
    }
    return NULL;
}

/**
 * @brief
*/
struct nlp_variable_t * nlp_clone_variable(struct nlp_variable_t *variable)
{
    struct nlp_variable_t *src[MAX_NEST];
    struct nlp_variable_t *dst[MAX_NEST];
    int sp;

    if (variable == NULL) return NULL;
    sp = 0;
    src[sp] = variable;

    while (1)
    {
        if ((dst[sp] = nlp_create_variable(src[sp]->name, src[sp]->type, src[sp]->size, src[sp]->dim, src[sp]->min, src[sp]->max)) == NULL) return NULL;
        if (sp > 0)
        {
            nlp_add_member(dst[sp - 1], dst[sp]);
        }
        if (src[sp]->type == NLP_TYPE_STRUCT)
        {
            src[sp + 1] = src[sp]->member;
            sp++;
        }
        else if (sp > 0)
        {
            if (src[sp]->next != NULL)
            {
                src[sp] = src[sp]->next;
            }
            else
            {
                sp--;
                if (sp > 0)
                {
                    if (src[sp]->next != NULL)
                    {
                        src[sp] = src[sp]->next;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    return dst[0];
}

int nlp_dispose_variable(struct nlp_variable_t *variable)
{
    struct nlp_variable_t *stack[MAX_NEST];
    int sp;

    if (variable == NULL)
    {
        return NLP_ERR_NULL_POINTER;
    }

    
}


/**
 * @brief find variable
*/
struct nlp_variable_t * nlp_find_variable(struct nlp_variable_list_t *variable_list, char *name)
{
    struct nlp_variable_list_t *head, *i;

    head = variable_list;
    for (i = head->next; i != head; i = i->next)
    {
        if (strcmp(i->v->name, name) == 0)
        {
            return i->v;
        }
    }
    return NULL;
}


/**
 * @brief add variable to valiable list.
 * @param variable_list
 * @param variable
 * @return
*/
int nlp_add_variable(struct nlp_variable_list_t *variable_list, struct nlp_variable_t *variable)
{
    if (nlp_find_variable(variable_list, variable->name) != NULL)
    {
        return NLP_ERR_VARIABLE_EXISTS;
    }

    return nlp_append_variable_list(variable_list, variable);
}


/**
 * @brief
*/
int nlp_append_variable_list(struct nlp_variable_list_t *variable_list, struct nlp_variable_t *variable)
{
    struct nlp_variable_list_t *vl;

    if (variable_list == NULL) return NLP_ERR_NULL_POINTER;
    if (variable == NULL) return NLP_ERR_NULL_POINTER;
    if ((vl = malloc(sizeof(struct nlp_variable_list_t))) == NULL) return NLP_ERR_NOMEM;

    vl->next = variable_list;
    vl->prev = variable_list->prev;
    variable_list->prev->next = vl;
    variable_list->prev = vl;

    return NLP_NOERR;
}

int nlp_calc_struct_size(struct nlp_variable_t *variable)
{
    struct nlp_variable_t *stack[MAX_NEST];
    int sp;
    int count;
    int i;

    if (variable == NULL) return NLP_ERR_NULL_POINTER;
    sp = 0;
    stack[sp] = variable;

    while (1)
    {
        if (stack[sp]->type == NLP_TYPE_STRUCT)
        {
            stack[sp]->size = 0;
            stack[sp + 1] = stack[sp]->member;
            sp++;
        }
        else if (sp > 0)
        {
            count = 1;
            for (i = 0; i < stack[sp]->dim; i++)
            {
                count = count * (stack[sp]->max[i] - stack[sp]->min[i] + 1);
            }
            stack[sp - 1]->size += stack[sp]->size * count;
            if (stack[sp]->next != NULL)
            {
                stack[sp] = stack[sp]->next;
            }
            else
            {
                sp--;
                if (sp > 0)
                {
                    count = 1;
                    for (i = 0; i < stack[sp]->dim; i++)
                    {
                        count = count * (stack[sp]->max[i] - stack[sp]->min[i] + 1);
                    }
                    stack[sp - 1]->size += stack[sp]->size * count;
                    if (stack[sp]->next != NULL)
                    {
                        stack[sp] = stack[sp]->next;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    return NLP_NOERR;
}

void *nlp_current_pointer();

int nlp_forward_reference(struct nlp_decode_variable_t *v)
{
    struct nlp_variable_t *cur;
    int next_node = 0;
    int i;

    cur = v->current;
    if (cur->dim == 0)
    {
        next_node = 1;
    }
    else
    {
        next_node = 1;
        for (i = 0; i < cur->dim; i++)
        {
            if (cur->cur[i] != cur->max[i])
            {
                next_node = 0;
                break;
            }
        }
    }
    if (next_node)
    {
        for (i = 0; i < cur->dim; i++)
        {
            cur->cur[i] = cur->min[i];
        }
        if (cur->next != NULL)
        {
            cur = cur->next;
            while (cur->type == NLP_TYPE_STRUCT)
            {
                v->variable_stack[v->variable_sp] = cur;
                v->variable_sp++;
                cur = cur->member;
            }
            v->current = cur;
        }
        else
        {
            while (1)
            {
                if (v->variable_sp == 0)
                {
                    return 1;
                }
                v->variable_sp--;
                cur = v->variable_stack[v->variable_sp];
                if (cur->dim == 0)
                {
                    next_node = 1;
                }
                else
                {
                    next_node = 1;
                    for (i = 0; i < cur->dim; i++)
                    {
                        if (cur->cur[i] != cur->max[i])
                        {
                            next_node = 0;
                            break;
                        }
                    }
                }
                if (next_node == 0)
                {
                    cur->cur[0]++;
                    for (i = 0; i < cur->dim; i++)
                    {
                        if (cur->cur[i] > cur->max[i])
                        {
                            cur->cur[i] = cur->min[i];
                            cur->cur[i + 1]++;
                        }
                    }
                    v->variable_stack[v->variable_sp] = cur;
                    v->variable_sp++;
                    cur = cur->member;
                    while (cur->type == NLP_TYPE_STRUCT)
                    {
                        v->variable_stack[v->variable_sp] = cur;
                        v->variable_sp++;
                        cur = cur->member;
                    }
                    v->current = cur;
                    break;
                }
                else
                {
                    for (i = 0; i < cur->dim; i++)
                    {
                        cur->cur[i] = cur->min[i];
                    }
                    if (cur->next != NULL)
                    {
                        cur = cur->next;
                        while (cur->type == NLP_TYPE_STRUCT)
                        {
                            v->variable_stack[v->variable_sp] = cur;
                            v->variable_sp++;
                            cur = cur->member;
                        }
                        v->current = cur;
                        break;
                    }
                }
            }
            v->current = cur;
        }
    }
    else
    {
        cur->cur[0]++;
        for (i = 0; i < cur->dim; i++)
        {
            if (cur->cur[i] > cur->max[i])
            {
                cur->cur[i] = cur->min[i];
                cur->cur[i + 1]++;
            }
        }
    }
    printf("VARIABLE: ");
    for (i = 0; i < v->variable_sp; i++){
        printf("%s", v->variable_stack[i]->name);
        if (v->variable_stack[i]->dim > 0)
        {
            printf("(");
            for (int d = 0; d < v->variable_stack[i]->dim; d++)
            {
                printf("%d%s", v->variable_stack[i]->cur[d], d < v->variable_stack[i]->dim - 1 ? "," : "");
            }
            printf(")");
        }
        printf(".");
    }
    printf("%s", v->current->name);
    if (v->current->dim > 0)
    {
        printf("(");
        for (int d = 0; d < v->current->dim; d++)
        {
            printf("%d%s", v->current->cur[d], d < v->current->dim - 1 ? "," : "");
        }
        printf(")");
    }
    printf("\n");
    return 0;
}