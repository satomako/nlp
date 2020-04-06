%{
/*
 * nlp.y
 * Create: 2017-04-30
 */
#define YYLEX_PARAM &yylval, &yylloc
static int nlp_yyint;
int yylex();
%}

%locations
%token	EQ LP RP CM NUMMUL MULMUL
%token	INTEGER REAL STRING SKIPPER
%token	IDENTIFIER SEPARATOR
%token	HEAD TAIL TERM
%token	BAD

%%
namelist		: HEAD list TAIL
			{
				YYACCEPT;
			}
			;

list			: /* empty */
			{
#ifdef __DEBUG_YACC__
				printf("YACC:list.empty: %s\n", yytext);
#endif
			}
			| list let
			{
#ifdef __DEBUG_YACC__
				printf("YACC:list.list let: %s\n", yytext);
#endif
			}
			| error TERM {yyerrok; fprintf(stderr, "list\n"); yyclearin;}
			;


let			: var_name EQ val_list TERM
			{
#ifdef __DEBUG_YACC__
				printf("YACC:let: %s\n", yytext);
#endif
				nlp_value_decode();
			}
			;


var_name		: IDENTIFIER
			{
#ifdef __DEBUG_YACC__
				printf("YACC:var_name.IDENTIFIER: %s\n", yytext);
				printf("YACC:lineno=%d\n", yylineno);
#endif
				nlp_set_addr_dimension(0);
			}
			| IDENTIFIER LP addr_list RP
			{
#ifdef __DEBUG_YACC__
				printf("YACC:var_name.addr_list: %s\n", yytext);
				printf("YACC:lineno=%d\n", yylineno);
				for (nlp_yyint = 0; nlp_yyint < nlp_get_addr_dimension(); nlp_yyint++) {
					if (nlp_yyint > NLP_MAX_DIMENSION - 1) break;
					printf("YACC:dim=%d, %d ", nlp_yyint, nlp_get_addr(nlp_yyint + 1));
				}
				printf("\n");
#endif
			}
			;


addr_list		: INTEGER
			{
#ifdef __DEBUG_YACC__
				printf("YACC:addr_list.INTEGER: %s\n", yytext);
#endif
				nlp_set_addr_dimension(1);
				nlp_set_addr(nlp_get_addr_dimension(), atoi(yytext));
			}
			| addr_list CM INTEGER
			{
#ifdef __DEBUG_YACC__
				printf("YACC:addr_list.CM INTEGER: %s\n", yytext);
#endif
				nlp_incr_addr_dimension();
				if (nlp_get_addr_dimension() > NLP_MAX_DIMENSION) {
					nlp_log("ERROR: max address dimension exceeds\n");
/*FIXME*/
#ifdef __DEBUG_YACC__
					printf("YACC:addr_dimension=%d\n", nlp_get_addr_dimension());
#endif
				} else {
					nlp_set_addr(nlp_get_addr_dimension(), atoi(yytext));
				}
			}
			;


val_list		: scalar
			| CM
			| LP val_list RP
			| multiplier LP val_list RP
			| val_list scalar
			| val_list CM
			| val_list LP val_list RP
			| val_list multiplier LP val_list RP
			;


scalar			: INTEGER
			{
#ifdef __DEBUG_YACC__
				printf("YACC:scalar.INTEGER: %s\n", yytext);
#endif
			}
			| REAL
			{
#ifdef __DEBUG_YACC__
				printf("YACC:scalar.REAL: %s\n", yytext);
#endif
			}
			| STRING
			{
#ifdef __DEBUG_YACC__
				printf("YACC:scalar.STRING: %s\n", nlp_get_string());
#endif
			}
			| SKIPPER
			{
#ifdef __DEBUG_YACC__
				printf("YACC:scalar.SKIPPER: %s\n", yytext);
#endif
			}
			| multiplier INTEGER
			{
#ifdef __DEBUG_YACC__
				printf("YACC:scalar.multiplier INTEGER: %s\n", yytext);
#endif
			}
			| multiplier REAL
			{
#ifdef __DEBUG_YACC__
				printf("YACC:scalar.multiplier REAL: %s\n", yytext);
#endif
			}
			| multiplier STRING
			{
#ifdef __DEBUG_YACC__
				printf("YACC:scalar.multiplier STRING: %s\n", nlp_get_string());
#endif
			}
			;


multiplier		: NUMMUL
			{
#ifdef __DEBUG_YACC__
				printf("YACC:multiplier.NUMMUL: %s\n", yytext);
#endif
			}
			| MULMUL
			{
#ifdef __DEBUG_YACC__
				printf("YACC:multiplier.MULMUL: %s\n", yytext);
#endif
			}
			;
