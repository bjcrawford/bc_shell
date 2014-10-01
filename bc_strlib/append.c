/* File: append.c
   Author: Brett Crawford
   Date: 2014-09-22
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"
#include <stdio.h>

/* Reports a file open error
   Input: character pointer, name of the file attempted to be open
   Output: integer, error code for file open error */
char *append(char *s1, char *s2)
{
	int l = strlen(s1) + strlen(s2) + 1;
	char *ns;
	if((ns = calloc(l, sizeof(char))) == NULL)
	{
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
