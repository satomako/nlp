#include <stdio.h>
#include "nlp.h"

int main(int ac, char *av[])
{
    struct nlp_t c;
    int count;

    nlp_init(&c);

    count = nlp_value_list_count(&c);
    printf("nlp_value_list_count = %d\n", count);
    _nlp_print_value_list(&c);
    printf("--------------------\n");

    nlp_add_value_list(&c);
    count = nlp_value_list_count(&c);
    printf("nlp_value_list_count = %d\n", count);
    _nlp_print_value_list(&c);
    printf("--------------------\n");

    nlp_add_value_list(&c);
    count = nlp_value_list_count(&c);
    printf("nlp_value_list_count = %d\n", count);
    _nlp_print_value_list(&c);
    printf("--------------------\n");

    nlp_remove_value_list_first(&c);
    count = nlp_value_list_count(&c);
    printf("nlp_value_list_count = %d\n", count);
    _nlp_print_value_list(&c);
    printf("--------------------\n");

    nlp_remove_value_list_first(&c);
    count = nlp_value_list_count(&c);
    printf("nlp_value_list_count = %d\n", count);
    _nlp_print_value_list(&c);
    printf("--------------------\n");

    return 0;
}