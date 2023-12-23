TARGET     = nlp.exe
LIBRARY    = libnlp.a

VPATH      = ../../nlp:../../include:../../test

LEX        = flex
LEXFLAGS   =
YACC       = bison
YACCFLAGS  = --locations -v -b nlp
CC         = gcc
LD         = ld
AR         = ar
ARFLAGS    = -rus
COPTS      = -O0 -g
CFLAGS     = -D__DEBUG_YACC__ -D__DEBUG_LEX__ -DYY_SKIP_YYWRAP $(COPTS) -I../../include
LDFLAGS    = $(COPTS)
LIBS       = 
OBJS       = nlp.lex.o nlp.o nlp_variable.o nlp_util.o

.SUFFIXES:
.SUFFIXES: .c .o

all: $(TARGET)

$(TARGET): nlp.c $(LIBRARY)
	$(CC) -o $@ $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.c.o:
	echo $<
	$(CC) -c $(CFLAGS) $<

nlp.o: nlp.c
nlp.lex.o: nlp.lex.c nlp.tab.c

nlp.lex.c: nlp.l nlp.tab.c
	$(LEX) $(LEXFLAGS) -o $@ $<
nlp.tab.c: nlp.y
	$(YACC) $(YACCFLAGS) $<

clean:
	 rm -f $(LIBRARY) nlp.lex.c nlp.tab.c *.o
