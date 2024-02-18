#include <stdio.h>
#include "nlp.h"

extern int yyparse();
int set_variable(struct nlp_t *c);

int main(int ac, char *av[])
{
    struct nlp_t c;
    int result;

    nlp_init(&c);
    //nlp_set_dryrun(&c, 1);
    result = set_variable(&c);
    yyparse();
    result = nlp_decode(&c);
    nlp_free_value_list(&c);
    return 0;
}

int set_variable(struct nlp_t *c)
{
    return NLP_NOERR;
}