LIBRARY    = libnlp.a

VPATH      = ../../nlp:../../include:../../test

LEX        = flex
LEXFLAGS   =
YACC       = bison
YACCFLAGS  = --locations --defines --file-prefix=nlp --verbose
YACCFLAGS_INTERNAL  = --locations --defines --file-prefix=nlp_internal --verbose
CC         = gcc
LD         = ld
AR         = ar
ARFLAGS    = -r
COPTS      = -O0 -g
#CFLAGS     = -D__DEBUG_YACC__ -D__DEBUG_LEX__ -DYY_SKIP_YYWRAP $(COPTS) -I../../include -I.
CFLAGS     = -D__DEBUG_YACC__ -D__DEBUG_LEX__ $(COPTS) -I../../include -I.
LDFLAGS    = $(COPTS)
LIBS       = 
#OBJS       = nlp.lex.o nlp.tab.o nlp.o nlp_variable.o nlp_value.o nlp_util.o
OBJS       = nlp.lex.o nlp.tab.o nlp.o nlp_variable.o nlp_value.o nlp_util.o nlp_internal.lex.o nlp_internal.tab.o

.SUFFIXES:
.SUFFIXES: .c .o

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

nlp.lex.o: nlp.lex.c nlp.h nlp.tab.c
nlp.tab.o: nlp.tab.c nlp.h
nlp.o: nlp.c nlp.h nlp.tab.c
nlp_variable.o: nlp_variable.c nlp.h
nlp_value.o: nlp_value.c nlp.h
nlp_util.o: nlp_util.c nlp.h

nlp.lex.c: nlp.l nlp.h nlp.tab.c
	$(LEX) $(LEXFLAGS) -o $@ $<
nlp.tab.c: nlp.y nlp.h
	$(YACC) $(YACCFLAGS) $<

nlp_internal.lex.c: nlp_internal.l nlp_internal.tab.c
	$(LEX) $(LEXFLAGS) -o $@ $<
nlp_internal.tab.c: nlp_internal.y
	$(YACC) $(YACCFLAGS_INTERNAL) $<

.c.o:
	echo $<
	$(CC) -c $(CFLAGS) $<

clean:
	 rm -f $(LIBRARY) nlp.lex.c nlp.tab.c *.o
