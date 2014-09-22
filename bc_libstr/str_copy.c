#include "bc_libstr.h"
#include <stdio.h>

char *str_copy(char *s)
{
	int l = strlen_bc(s) + 1;
	char *ns;
	if((ns = malloc(l * sizeof(char))) == NULL)
		return NULL;
	char *p = ns;
	while(*s != '\0')
		*p++ = *s++;
	*p = '\0';

	return ns;
}