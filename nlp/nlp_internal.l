%option prefix="zz"
%option noinput nounput 8bit
%option yylineno
%option reentrant bison-bridge bison-locations
%option never-interactive

%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#define YY_NO_UNISTD_H
#ifndef ZZSTYPE
#define YYSTYPE ZZSTYPE
#define YYLTYPE ZZLTYPE
#endif
#include "nlp_internal.tab.h"
#define yyterminate() return END;

#define YY_USER_ACTION                              \
    yylloc->first_line = yylloc->last_line;         \
    yylloc->first_column = yylloc->last_column;     \
    if (yylloc->last_line == yylineno)              \
    {                                               \
        yylloc->first_column = yylloc->last_column; \
    }                                               \
    else                                            \
    {                                               \
        yylloc->last_line = yylineno;               \
        yylloc->first_column = yytext + yyleng - strrchr(yytext, '\n'); \
    }

%}

ND		[a-zA-Z_]
NM		[1-9]
DG		[0-9]
AD		[0-9a-zA-Z_]
SN		[+-]

%x DEF

%%

<INITIAL>"int8" {
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return INT8;
}

<INITIAL>"int32" {
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return INT32;
}

<INITIAL>"int64" {
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return INT64;
}

<INITIAL>"real32" {
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return REAL32;
}

<INITIAL>"real64" {
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return REAL64;
}

<INITIAL>"string" {
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return STRING;
}

<INITIAL>"typedef" {
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return TYPEDEF;
}

<INITIAL>"type" {
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return TYPE;
}

<DEF>{SN}?{NM}{DG}* {
printf("LEX:%s\n", yytext);
    return INTEGER;
}

<DEF>{ND}{AD}* {
printf("LEX:%s\n", yytext);
    return IDENTIFIER;
}

<DEF>"(" {
printf("LEX:%s\n", yytext);
    return LP;
}

<DEF>")" {
printf("LEX:%s\n", yytext);
    return RP;
}

<DEF>"{" {
    BEGIN(INITIAL);
printf("LEX:%s\n", yytext);
    return LB;
}

<INITIAL>"}" {
printf("LEX:%s\n", yytext);
    return RB;
}

<DEF>":" {
printf("LEX:%s\n", yytext);
    return COLON;
}

<DEF>";" {
    BEGIN(INITIAL);
printf("LEX:%s\n", yytext);
    return SM;
}

<DEF>"," {
printf("LEX:%s\n", yytext);
    return CM;
}

<INITIAL>\n ;
<DEF>\n ;

<INITIAL><<EOF>> {
    return END;
}
<DEF><<EOF>> {
    return END;
}
%%