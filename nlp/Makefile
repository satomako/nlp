LIBRARY    = libnlp.a

VPATH      = ../../nlp:../../include:../../test

LEX        = flex
LEXFLAGS   =
YACC       = bison
YACCFLAGS  = --locations -v -b nlp
CC         = gcc
LD         = ld
AR         = ar
ARFLAGS    = -r
COPTS      = -O0 -g
CFLAGS     = -D__DEBUG_YACC__ -D__DEBUG_LEX__ -DYY_SKIP_YYWRAP $(COPTS) -I../../include
LDFLAGS    = $(COPTS)
LIBS       = 
OBJS       = nlp.lex.o nlp.tab.o nlp.o nlp_variable.o nlp_value.o nlp_util.o

.SUFFIXES:
.SUFFIXES: .c .o

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.c.o:
	echo $<
	$(CC) -c $(CFLAGS) $<

nlp.lex.o: nlp.lex.c nlp.h nlp.tab.c
nlp.tab.o: nlp.tab.c nlp.h
nlp.o: nlp.c nlp.h
nlp_variable.o: nlp_variable.c nlp.h
nlp_value.o: nlp_value.c nlp.h
nlp_util.o: nlp_util.c nlp.h

nlp.lex.c: nlp.l nlp.h nlp.tab.c
	$(LEX) $(LEXFLAGS) -o $@ $<
nlp.tab.c: nlp.y nlp.h
	$(YACC) $(YACCFLAGS) $<

clean:
	 rm -f $(LIBRARY) nlp.lex.c nlp.tab.c *.o
