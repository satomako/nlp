#include <stdio.h>
typedef void *yyscan_t;
#include "nlp.h"
#define YYSTYPE ZZSTYPE
#define YYLTYPE ZZLTYPE
#include "nlp_internal.tab.h"
#include "nlp_internal.lex.h"

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