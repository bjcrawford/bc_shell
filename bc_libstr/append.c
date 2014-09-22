#include "bc_libstr.h"
#include <stdio.h>

char *append(char *s1, char *s2)
{
	int l = strlen_bc(s1) + strlen_bc(s2) + 1;
	char *ns;
	if((ns = malloc(l * sizeof(char))) == NULL)
	{
		printf(stderr, "Error allocating memory for append\n");
		return NULL;
	}
	char *p = ns;
	while(*s1 != '\0')
		*p++ = *s1++;
	while(*s2 != '\0')
		*p++ = *s2++;
	*p = '\0';

	return ns;
}
