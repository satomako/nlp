PROGRAM = libnlp.a

LEX        = flex
LEXFLAGS   =
YACC       = bison
YACcFLAGS  = --locations -v -b nlp
CC         = gcc
LD         = gcc
AR         = ar
ARFLAGS    = -rucs
COPTS      = -O0 -g
CFLAGS     = -D__DEBUG_YACC__ -DYY_SKIP_YYWRAP -DNLP_UNDERSCORE $(COPTS)
#CFLAGS     =                  -DYY_SKIP_YYWRAP -DNLP_UNDERSCORE $(COPTS)
LDFLAGS    = $(COPTS)
LIBS       = 

SRC        = ../src
OBJS       = nlp.lex.o nlp_state.o nlp_value.o nlp_variable.o nlp_util.o nlp_log.o nlp.o

.SUFFIXES:

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

nlp.o: $(SRC)/nlp.c $(SRC)/nlp.h
	$(CC) -c $(CFLAGS) $(SRC)/nlp.c

nlp_value.o: $(SRC)/nlp_value.c $(SRC)/nlp_value.h
	$(CC) -c $(CFLAGS) $(SRC)/nlp_value.c

nlp_variable.o: $(SRC)/nlp_variable.c $(SRC)/nlp_variable.h
	$(CC) -c $(CFLAGS) $(SRC)/nlp_variable.c

nlp_state.o: $(SRC)/nlp_state.c $(SRC)/nlp_state.h
	$(CC) -c $(CFLAGS) $(SRC)/nlp_state.c

nlp_util.o: $(SRC)/nlp_util.c $(SRC)/nlp_util.h
	$(CC) -c $(CFLAGS) $(SRC)/nlp_util.c

nlp_log.o: $(SRC)/nlp_log.c $(SRC)/nlp_log.h
	$(CC) -c $(CFLAGS) $(SRC)/nlp_log.c

nlp.lex.o: nlp.lex.c nlp.tab.c $(SRC)/nlp.h $(SRC)/nlp_value.h
	$(CC) -c $(CFLAGS) nlp.lex.c

nlp.lex.c: $(SRC)/nlp.l nlp.tab.c
	$(LEX) $(LEXFLAGS) -o $@ $(SRC)/nlp.l

nlp.tab.c: $(SRC)/nlp.y
	$(YACC) $(YACCFLAGS) $(SRC)/nlp.y

clean:
	rm -f $(PROGRAM) nlp.lex.c nlp.tab.c nlp.output *.o
