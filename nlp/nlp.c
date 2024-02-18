#include <stdio.h>
#include <string.h>

#include "nlp.h"

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
    if (c == NULL) return NLP_ERR_BAD_ARGUMENT;
    return NLP_NOERR;
}