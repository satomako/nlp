#include <stdio.h>

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE zz_scan_string(char * str);
extern YY_BUFFER_STATE zz_scan_buffer(char * str, int size);
extern void zz_delete_buffer(YY_BUFFER_STATE buffer);

extern int zzparse();

int zzwrap()
{
    return 1;
}

void zzerror(const char *str)
{
    printf("XXX %s\n", str);
}

int main(int ac, char *av[])
{
    char input[] = "int8 hoge(10);  int8 hige(10:10);\0";
    //YY_BUFFER_STATE buffer = zz_scan_buffer(input, sizeof(input));
    //zzparse();

    YY_BUFFER_STATE buffer = zz_scan_string(input);
    zzparse();
    zz_delete_buffer(buffer);
    return 0;
}