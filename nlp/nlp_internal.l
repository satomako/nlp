%option prefix="zz"
%option noinput nounput 8bit nodefault
%option yylineno
%option reentrant bison-bridge bison-locations
%option never-interactive

%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#define YY_NO_UNISTD_H
#include "nlp_internal.tab.h"
#ifndef ZZSTYPE
#define YYSTYPE ZZSTYPE
#endif
#ifndef ZZLTYPE_IS_DECLARED
#define YYLTYPE ZZLTYPE
#endif
#define yyterminate() return END;
#include "nlp.h"

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
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, INT8, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return INT8;
}

<INITIAL>"int32" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, INT32, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return INT32;
}

<INITIAL>"int64" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, INT64, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return INT64;
}

<INITIAL>"real32" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, REAL32, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return REAL32;
}

<INITIAL>"real64" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, REAL64, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return REAL64;
}

<INITIAL>"string" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, STRING, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return STRING;
}

<INITIAL>"typedef" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, TYPEDEF, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return TYPEDEF;
}

<INITIAL>"type" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, TYPE, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(DEF);
printf("LEX:%s\n", yytext);
    return TYPE;
}

<DEF>{SN}?{NM}{DG}* {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, INTEGER, yytext, (struct ZZLTYPE **)&yylloc);
printf("LEX:%s\n", yytext);
    return INTEGER;
}

<DEF>{ND}{AD}* {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, IDENTIFIER, yytext, (struct ZZLTYPE **)&yylloc);
printf("LEX:%s\n", yytext);
    return IDENTIFIER;
}

<DEF>"(" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, LP, yytext, (struct ZZLTYPE **)&yylloc);
printf("LEX:%s\n", yytext);
    return LP;
}

<DEF>")" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, RP, yytext, (struct ZZLTYPE **)&yylloc);
printf("LEX:%s\n", yytext);
    return RP;
}

<DEF>"{" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, LB, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(INITIAL);
printf("LEX:%s\n", yytext);
    return LB;
}

<INITIAL>"}" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, RB, yytext, (struct ZZLTYPE **)&yylloc);
printf("LEX:%s\n", yytext);
    return RB;
}

<DEF>":" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, COLON, yytext, (struct ZZLTYPE **)&yylloc);
printf("LEX:%s\n", yytext);
    return COLON;
}

<DEF>";" {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, SM, yytext, (struct ZZLTYPE **)&yylloc);
    BEGIN(INITIAL);
printf("LEX:%s\n", yytext);
    return SM;
}

<DEF>"," {
    struct nlp_t *_wc;
    _wc = zzget_extra(yyscanner);
    nlp_add_variable_definition_token(_wc, CM, yytext, (struct ZZLTYPE **)&yylloc);
printf("LEX:%s\n", yytext);
    return CM;
}

<INITIAL><<EOF>> {
    return END;
}
<DEF><<EOF>> {
    return END;
}

<INITIAL>[ \t\r\n] ;
<DEF>[ \t\r\n] ;

%%