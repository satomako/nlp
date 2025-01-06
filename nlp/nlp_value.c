/*
 * nlp_value.c
 * This file is part of nlp.
 * This file handles variable decoding.
 */
#include <stdio.h>
#include <stdlib.h>
#include "nlp.tab.h"
#include "nlp.lex.h"
#include "nlp.h"

void _nlp_dump_values(struct nlp_t *c)
{
    struct nlp_value_list_t *vl;
    if (c == NULL)
        return;
    for (vl = c->value_list_head.next; vl; vl = vl->next)
    {
        printf("-------------\n");
        _nlp_print_values(vl->list_head.next);
    }
}

void _nlp_print_values(struct nlp_value_t *head)
{
    struct nlp_value_t *i;
    for (i = head; i; i = i->next)
    {
        printf("%s\n", i->value);
    }
}

void _nlp_print_value_list(struct nlp_t *c)
{
    struct nlp_value_list_t *vl;
    for (vl = c->value_list_head.next; vl; vl = vl->next)
    {
        printf("%p %p\n", vl, vl->next);
    }
}

/**
 * @brief count values in the value list
 * @param c pointer of nlp context
 * @return count of values in the value list
 */
int nlp_value_list_count(struct nlp_t *c)
{
    int count;
    struct nlp_value_list_t *vl;
    count = 0;
    for (vl = c->value_list_head.next; vl != NULL; vl = vl->next)
    {
        count++;
    }
    return count;
}

/**
 * @brief create new value list
 * @return pointer of created value list
 */
struct nlp_value_list_t *nlp_create_value_list()
{
    struct nlp_value_list_t *vl;
    vl = calloc(1, sizeof(struct nlp_value_list_t));
    return vl;
}

/**
 * @brief add value list to the context
 * @param c pointer of nlp context
 * @return NLP_NOERR at success, otherwise NLP_ERR_XXX
 */
int nlp_add_value_list(struct nlp_t *c)
{
    struct nlp_value_list_t *i;
    struct nlp_value_list_t *vl;

    if (c == NULL)
        return NLP_ERR_BAD_STATUS;
    vl = nlp_create_value_list();
    if (vl == NULL)
        return NLP_ERR_NOMEM;
    for (i = &c->value_list_head; i; i = i->next)
    {
        if (i->next == NULL)
        {
            i->next = vl;
            break;
        }
    }
    c->current_value_list_head = &vl->list_head;
    c->value_state = NLP_VALUE_STATE_IDENTIFIER;
    return NLP_NOERR;
}

/**
 * @brief remove first value from the context
 * @param c pointer of nlp context
 */
void nlp_remove_value_list_first(struct nlp_t *c)
{
    struct nlp_value_list_t *vl;

    if (c->value_list_head.next == NULL)
        return;
    vl = c->value_list_head.next;
    c->value_list_head.next = c->value_list_head.next->next;
    nlp_free_values(vl->list_head.next);
    free(vl);
}

/**
 * @brief free value list
 * @param c pointer of nlp context
 */
void nlp_free_value_list(struct nlp_t *c)
{
    struct nlp_value_list_t *i;

    if (c == NULL)
        return;
    while (c->value_list_head.next)
    {
        nlp_remove_value_list_first(c);
    }
}

/**
 * @brief free values
 * @param vl pointer of value list
 */
void nlp_free_values(struct nlp_value_t *vl)
{
    struct nlp_value_t *i;
    struct nlp_value_t *j;
    i = vl;
    while (i)
    {
        j = i->next;
        if (i->value)
        {
            if (i->value)
                free(i->value);
        }
        free(i);
        i = j;
    }
}

/**
 * @brief add value to the value list
 * @param c pointer of nlp context
 * @param vtype value type
 * @param value value
 * @param yyl pointer of location
 * @return NLP_NOERR at success, otherwise NLP_ERR_XXX
 */
int nlp_add_value(struct nlp_t *c, int vtype, char *value, struct YYLTYPE **yyl)
{
    struct nlp_value_t *i;
    struct nlp_value_t *v;

    if (c == NULL)
        return NLP_ERR_BAD_ARGUMENT;
    for (i = c->current_value_list_head; i; i = i->next)
    {
        if (i->next == NULL)
        {
            break;
        }
    }

    v = calloc(1, sizeof(struct nlp_value_t));
    if (v == NULL)
        return NLP_ERR_BAD_STATUS;

    v->value = t_strdup(value);
    if (v->value == NULL)
        return NLP_ERR_BAD_STATUS;
    v->type = vtype;
    v->first_line = (*yyl)->first_line;
    v->first_column = (*yyl)->first_column;
    i->next = v;
    return NLP_NOERR;
}

/**
 * @brief reset scanning string
 * @param c pointer of nlp context
 */
void nlp_string_reset(struct nlp_t *c)
{
    c->scanning_string[0] = '\0';
    c->scanning_string_length = 0;
}

/**
 * @brief append character to scanning string
 * @param c pointer of nlp context
 * @param chr character
 * @return 0 at success, otherwise 1
 */
int nlp_string_append(struct nlp_t *c, char chr)
{
    if (c->scanning_string_length == NLP_MAX_STRING_LENGTH)
    {
        return 1;
    }
    c->scanning_string[c->scanning_string_length++] = chr;
    c->scanning_string[c->scanning_string_length] = '\0';
    return 0;
}