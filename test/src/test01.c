#include <stdio.h>
#include "nlp.tab.h"
#include "nlp.lex.h"
#include "nlp.h"

extern int yyparse();

int main(int ac, char *av[])
{
    struct nlp_t c;

    nlp_init(&c);
    // nlp_set_dryrun(&c, 1);
    yyparse(c.scanner);
    printf("--------------------\n");
    _nlp_print_value_list(&c);
    printf("--------------------\n");
    _nlp_dump_values(&c);
    nlp_free_value_list(&c);
    yylex_destroy(c.scanner);
    return 0;
}