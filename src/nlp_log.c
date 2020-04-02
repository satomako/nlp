#include <stdio.h>
#include <stdarg.h>
#include "nlp_log.h"



static FILE *log;

void nlp_log_set(FILE *logfile)
{
	log = logfile;
}


void nlp_log(char *format, ...)
{
	va_list args;

	if (log == NULL) return;
	va_start(args, format);
	vfprintf(log, format, args);
	va_end(args);
}
