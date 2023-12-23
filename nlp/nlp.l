%top{
#include <stdint.h>
}


%{
#include "nlp.tab.c"

#define YY_NO_UNISTD_H
int yylineno;
int yycolumn;

%}

ND		[a-zA-Z_]
NM		[1-9]
DG		[0-9]
AD		[0-9a-zA-Z_]
SN		[+-]
EP		[deDE]{SN}?{DG}+
FC		{DG}*"."{DG}+
FC2		{DG}+"."
SM      ";"

%start NAMEL
%start VNAME
%start SSTR
%start DSTR
%start IFILE
%start COMMENT


%%

<NAMEL>\/ {
#ifdef __DEBUG_LEX__
    printf("LEX:TAIL:<NAMEL>/  %s\n", yytext);
#endif
    return TAIL;
}

<INITIAL>&{ND}{AD}+ {
#ifdef __DEBUG_LEX__
    printf("LEX:HEAD:<INITIAL>&{ND}{AD}+  %s\n", yytext);
#endif
    BEGIN NAMEL;
    return HEAD;
}

<NAMEL>; {
#ifdef __DEBUG_LEX__
    printf("LEX:TERM:<NAMEL>;  %s\n", yytext);
#endif
    return TERM;
}

<NAMEL>{ND}{AD}* {
#ifdef __DEBUG_LEX__
    printf("LEX:IDENTIFIER:<NAMEL>{ND}{AD}*  %s\n", yytext);
#endif
    BEGIN VNAME;
    return IDENTIFIER;
}

<VNAME>{ND}{AD}* {
#ifdef __DEBUG_LEX__
    printf("LEX:IDENTIFIER:<VNAME>{ND}{AD}*  %s\n", yytext);
#endif
    return IDENTIFIER;
}

<VNAME>"=" {
#ifdef __DEBUG_LEX__
    printf("LEX:EQ:<VNAME>=  %s\n", yytext);
#endif
    BEGIN NAMEL;
    return EQ;
}

<VNAME>"(" {
#ifdef __DEBUG_LEX__
    printf("LEX:LP:<VNAME>(  %s\n", yytext);
#endif
    return LP;
}

<VNAME>")" {
#ifdef __DEBUG_LEX__
    printf("LEX:RP:<VNAME>)  %s\n", yytext);
#endif
    return RP;
}

<VNAME>"." {
#ifdef __DEBUG_LEX__
    printf("LEX:DOT:<NAMEL>.  %s\n", yytext);
#endif
    return DOT;
}

<NAMEL>"(" {
#ifdef __DEBUG_LEX__
    printf("LEX:LP:<NAMEL>(  %s\n", yytext);
#endif
    return LP;
}

<NAMEL>")" {
#ifdef __DEBUG_LEX__
    printf("LEX:RP:<NAMEL>)  %s\n", yytext);
#endif
    return RP;
}

<VNAME>{SN}?{DG}+ {
#ifdef __DEBUG_LEX__
    printf("LEX:INTEGER:<VNAME>{SN}?{DG}+  %s\n", yytext);
#endif
    return INTEGER;
}

<VNAME>"," {
#ifdef __DEBUG_LEX__
    printf("LEX:CM:<VNAME>,  %s\n", yytext);
#endif
    return CM;
}

<NAMEL>"," {
#ifdef __DEBUG_LEX__
    printf("LEX:CM:<NAMEL>,  %s\n", yytext);
#endif
    return CM;
}

<INITIAL>[ \t] ;
<NAMEL>[ \t] ;
<INITIAL>\n {
    yycolumn = 1;
    yylineno++;
}
<NAMEL>\n {
    yycolumn = 1;
    yylineno++;
}
<COMMENT>\n {
    yycolumn = 1;
    yylineno++;
}
<NAMEL>^#[ \t]*include[ \t]*[<'\"] {
    BEGIN IFILE;
}
<IFILE>[^ \t\n>'\"] {
    {
        int c;
        while ((c = input() && c!= '\n'));
    }
    yylineno++;
}
<INITIAL><<EOF>> {
    yyterminate();
}
<NAMEL><<EOF>> {
    yyterminate();
}
<NAMEL>!.*\n ;	/* comment */
<NAMEL>#.*\n ;	/* comment */
<NAMEL>\/\* {
    BEGIN COMMENT;
}
<COMMENT>\*\/ {
    BEGIN NAMEL;
}
<COMMENT>. ;
<NAMEL>{SN}?{DG}+ {
#ifdef __DEBUG_LEX__
    printf("LEX:INTEGER:<NAMEL>{SN}?{DG}+  %s\n", yytext);
#endif
    return INTEGER;
}
<NAMEL>{NM}{DG}*\* {
#ifdef __DEBUG_LEX__
    printf("LEX:NUMMUL:<NAMEL>{NM}{DG}*\\*  %s\n", yytext);
#endif
    return NUMMUL;
}
<NAMEL>"**" {
#ifdef __DEBUG_LEX__
    printf("LEX:MULMUL:<NAMEL>**  %s\n", yytext);
#endif
    return MULMUL;
}
<NAMEL>{SN}?{DG}+{EP} {
#ifdef __DEBUG_LEX__
    printf("LEX:REAL:<NAMEL>{SN}?{DG}+{EP}  %s\n", yytext);
#endif
    return REAL;
}
<NAMEL>{SN}?{FC}+ {
#ifdef __DEBUG_LEX__
    printf("LEX:REAL:<NAMEL>{SN}?{FC}+  %s\n", yytext);
#endif
    return REAL;
}
<NAMEL>{SN}?{FC}+{EP} {
#ifdef __DEBUG_LEX__
    printf("LEX:REAL:<NAMEL>{SN}?{FC}+{EP}  %s\n", yytext);
#endif
    return REAL;
}
<NAMEL>{SN}?{FC2}+ {
#ifdef __DEBUG_LEX__
    printf("LEX:REAL:<NAMEL>{SN}?{FC2}+  %s\n", yytext);
#endif
    return REAL;
}
<NAMEL>{SN}?{FC2}+{EP} {
#ifdef __DEBUG_LEX__
    printf("LEX:REAL:<NAMEL>{SN}?{FC2}+{EP}  %s\n", yytext);
#endif
    return REAL;
}
<NAMEL>@+ {
#ifdef __DEBUG_LEX__
    printf("LEX:SKIPPER:<NAMEL>@+  %s\n", yytext);
#endif
    return SKIPPER;
}
<NAMEL>\" {
    BEGIN DSTR;
}
<NAMEL>' {
    BEGIN SSTR;
}
<DSTR>\" {
#ifdef __DEBUG_LEX__
    printf("LEX:STRING:<DSTR>\n");
#endif
    BEGIN NAMEL;
    return STRING;
}
<SSTR>' {
#ifdef __DEBUG_LEX__
    printf("LEX:STRING:<SSTR>\n");
#endif
    BEGIN NAMEL;
    return STRING;
}
<DSTR>. {
}
<SSTR>. {
}

%%
