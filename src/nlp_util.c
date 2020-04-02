/* nlp_util.c
 * 2017-04-30
 */

#include <stdio.h>
#include "nlp_util.h"

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
