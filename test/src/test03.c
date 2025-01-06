#include <stdio.h>
// #include "nlp.tab.h"
// #include "nlp.lex.h"
// #include "nlp.h"

// typedef struct yy_buffer_state *YY_BUFFER_STATE;
// extern YY_BUFFER_STATE zz_scan_string(char *str);
// extern YY_BUFFER_STATE zz_scan_buffer(char *str, int size);
// extern void zz_delete_buffer(YY_BUFFER_STATE buffer);

typedef void *yyscan_t;
#include "nlp.h"
#define YYSTYPE ZZSTYPE
#define YYLTYPE ZZLTYPE
#include "nlp_internal.tab.h"
#include "nlp_internal.lex.h"

// extern int zzparse();

/*
int zzwrap()
{
    return 1;
}

void zzerror(const char *str)
{
    printf("XXX %s\n", str);
}
*/

int main(int ac, char *av[])
{
    struct nlp_t c;

    extern int zzdebug;
    zzdebug = 1;
    nlp_init(&c);
    nlp_init_variable_definition(&c);
    nlp_decode_variable_definition_file(&c, "data/test.nlp");
    // nlp_decode_variable_definition_file(&c, "data/test_bad.nlp");
    printf("OK\n");
    return 0;
}