/* nlp_util.c
 * 2017-04-30
*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "nlp.h"


/**
 * @brief copy string
 * @param dst distination
 * @param src source
 * @param size size of distination
 * @return dst at success, NULL at failure
*/
char *t_strlcpy(char *dst, char *src, int size)
{
	char *dst_save = dst;
	if (size < 1) return NULL;
	while ((*dst = *src) && (size > 1)) {
		dst++;
		src++;
		size--;
	}
	*dst = '\0';
	return dst_save;
}

/**
 * @brief duplicate string
 * @param src source
 * @return duplicated string, NULL at failure
*/
char *t_strdup(char *src)
{
	int l;
	char *p;

	if (src == NULL) return NULL;
	l = strlen(src) + 1;
	p = malloc(l);
	if (p != NULL)
	{
		t_strlcpy(p, src, l);
	}
	return p;
}


/**
 * @brief copy string (fixed length buffer)
 * @param dst distination
 * @param src source
 * @param size size of distination
 * @param opt option
 * @return
*/
char *t_strlcpy_fix(char *dst, char *src, int size, int opt)
{
	char *dst_save = dst;
	if (size < 1) return NULL;
	if ((opt & NLP_STRING_TOUPPER) != 0) {
		while ((*dst = toupper(*src)) && (size > 0)) {
			dst++;
			src++;
			size--;
		}
	} else if ((opt & NLP_STRING_TOLOWER) != 0) {
		while ((*dst = tolower(*src)) && (size > 0)) {
			dst++;
			src++;
			size--;
		}
	} else {
		while ((*dst = *src) && (size > 0)) {
			dst++;
			src++;
			size--;
		}
	}
	if ((opt & NLP_STRING_PAD_SPACE) != 0) {
		while (size > 0) {
			*dst = ' ';
			size--;
		}
	} else {
		while (size > 0) {
			*dst = '\0';
			size--;
		}
	}
	return dst_save;
}