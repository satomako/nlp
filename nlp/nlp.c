#include <stdio.h>
#include <string.h>

#include "nlp.h"

int yywrap();
void yyerror(const char *);

int main(int ac, char* av[])
{
    return yyparse();
}

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
}