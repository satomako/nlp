%top{
#include <stdint.h>
}

%option prefix="zz"
%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#define YY_NO_UNISTD_H
#include "nlp_internal.tab.c"

int yylineno;
int yycolumn;
%}

ND		[a-zA-Z_]
NM		[1-9]
DG		[0-9]
AD		[0-9a-zA-Z_]
SN		[+-]

%s DEF

%%

<INITIAL>

<INITIAL>int8 {
    BEGIN DEF;
    return INT8;
}

<INITIAL>int32 {
    BEGIN DEF;
    return INT32;
}

<INITIAL>int64 {
    BEGIN DEF;
    return INT64;
}

<INITIAL>real32 {
    BEGIN DEF;
    return REAL32;
}

<INITIAL>real64 {
    BEGIN DEF;
    return REAL64;
}

<INITIAL>string {
    BEGIN DEF;
    return STRING;
}

<DEF>{SN}?{NM}{DG}* {
    return INTEGER;
}

<DEF>{ND}{AD}* {
    return IDENTIFIER;
}

<DEF>"(" {
    return LP;
}

<DEF>")" {
    return RP;
}

<DEF>":" {
    return COLON;
}

<DEF>";" {
    BEGIN INITIAL;
    return SM;
}

<DEF>"," {
    return CM;
}

%%