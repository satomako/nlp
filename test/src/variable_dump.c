#include <stdio.h>
#include "nlp.h"

void pnode(struct nlp_variable_t *);

int main(int ac, char *av[])
{
    struct nlp_variable_t *v, *v2, *v3;
    struct nlp_variable_t *member;
    struct nlp_variable_t *stack[NLP_MAX_MEMBER_DEPTH];
    struct nlp_decode_variable_t d;
    int min[NLP_MAX_DIMENSION];
    int max[NLP_MAX_DIMENSION];
    int sp;

    min[0] = 0;
    max[0] = 1;
/*
    v = nlp_create_variable("hoge", NLP_TYPE_STRUCT, 0, 0, 1, min, max);
    member = nlp_create_variable("int32_member", NLP_TYPE_INT32, 0, 0, 0, min, max);
    nlp_add_member(v, member);
    member = nlp_create_variable("real32_member", NLP_TYPE_REAL32, 0, 0, 0, min, max);
    nlp_add_member(v, member);
    v2 = nlp_create_variable("hoge_member", NLP_TYPE_STRUCT, 0, 0, 0, min, max);
    nlp_add_member(v, v2);
    member = nlp_create_variable("int64_member", NLP_TYPE_INT64, 0, 0, 0, min, max);
    nlp_add_member(v2, member);
    v3 = nlp_create_variable("hoge_member_member", NLP_TYPE_STRUCT, 0, 0, 0, min, max);
    nlp_add_member(v2, v3);
    member = nlp_create_variable("real64_member", NLP_TYPE_REAL64, 0, 0, 0, min, max);
    nlp_add_member(v3, member);
*/
    v = nlp_create_variable("STR1", NLP_TYPE_STRUCT, 0, 0, 1, min, max);
    v2 = nlp_create_variable("STR2", NLP_TYPE_STRUCT, 0, 0, 1, min, max);
    nlp_add_member(v, v2);
    member = nlp_create_variable("real32_member", NLP_TYPE_REAL32, 0, 0, 1, min, max);
    nlp_add_member(v2, member);
    member = nlp_create_variable("int64_member", NLP_TYPE_INT64, 0, 0, 1, min, max);
    nlp_add_member(v2, member);
    member = nlp_create_variable("real64_member", NLP_TYPE_REAL64, 0, 0, 1, min, max);
    nlp_add_member(v, member);
    sp = 0;
    nlp_calc_struct_size(v, stack, sp);
    pnode(v);

    v2 = nlp_clone_variable(v);
    pnode(v2);

    d.variable = v;
    d.current = v->member->member;
    d.variable_stack[0] = v;
    d.variable_stack[1] = v->member;
    d.variable_sp = 2;
    while (nlp_forward_reference(&d) == 0);

    return 0;
}

void pnode(struct nlp_variable_t *v)
{
    for (int i = 0; i < v->level * 2; i++)
    {
        printf(" ");
    }
    printf("%d  %s", v->type, v->name);
    if (v->dim > 0)
    {
        printf("(");
        for (int i = 0; i < v->dim; i++)
        {
            printf("%d:%d%s", v->min[i], v->max[i], v->dim - 1 == i ? "" : ",");
        }
        printf(")");
    }
    printf(" size:%d msize:%d\n", v->size, v->msize);
    if (v->type == NLP_TYPE_STRUCT)
    {
        struct nlp_variable_t *member;
        for (member = v->member; member != NULL; member = member->next)
        {
            pnode(member);
        }
    }
}