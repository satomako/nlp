#include <stdio.h>
#include "nlp.tab.h"
#include "nlp.lex.h"
#include "nlp.h"

extern int yyparse();
int set_variable(struct nlp_t *c);

int main(int ac, char *av[])
{
    struct nlp_t c;
    int result;

    nlp_init(&c);
    //  nlp_set_dryrun(&c, 1);
    fprintf(stderr, "ENTER\n");
    fprintf(stderr, "  scanner = %p\n", c.scanner);
    fprintf(stderr, "  &c      = %p\n", &c);
    fprintf(stderr, "  extra   = %p\n", yyget_extra(c.scanner));
    result = set_variable(&c);
    yyparse(c.scanner);
    result = nlp_decode(&c);
    nlp_free_value_list(&c);
    yylex_destroy(c.scanner);
    return 0;
}

int set_variable(struct nlp_t *c)
{
    struct nlp_variable_t *v, *v2;
    struct nlp_variable_t *stack[NLP_MAX_MEMBER_DEPTH];
    int sp;
    int min[NLP_MAX_DIMENSION];
    int max[NLP_MAX_DIMENSION];
    int i;

    for (i = 0; i < NLP_MAX_DIMENSION; i++)
    {
        min[i] = 0;
        max[i] = 0;
    }
    min[0] = 1;
    max[0] = 100;
    v = nlp_create_variable("HOGE", NLP_TYPE_STRUCT, 0, 0, 1, min, max);
    min[0] = 1;
    max[0] = 2;
    min[1] = 1;
    max[1] = 3;
    v2 = nlp_create_variable("HIGE", NLP_TYPE_INT32, 0, 0, 2, min, max);
    nlp_add_member(v, v2);
    v2 = nlp_create_variable("HAGE", NLP_TYPE_REAL32, 0, 0, 2, min, max);
    nlp_add_member(v, v2);
    sp = 0;
    nlp_calc_struct_size(v, stack, sp);
    nlp_add_variable(&c->variable_list_head, v);

    min[0] = 1;
    max[0] = 3;
    min[1] = 0;
    max[1] = 0;
    v = nlp_create_variable("B", NLP_TYPE_INT32, 0, 0, 1, min, max);
    sp = 0;
    nlp_calc_struct_size(v, stack, sp);
    nlp_add_variable(&c->variable_list_head, v);

    min[0] = 0;
    max[0] = 0;
    v = nlp_create_variable("C", NLP_TYPE_STRUCT, 0, 0, 0, min, max);
    v2 = nlp_create_variable("D", NLP_TYPE_INT32, 0, 0, 0, min, max);
    nlp_add_member(v, v2);
    sp = 0;
    nlp_calc_struct_size(v, stack, sp);
    nlp_add_variable(&c->variable_list_head, v);

    return NLP_NOERR;
}