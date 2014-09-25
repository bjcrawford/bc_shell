/* File: str-copy.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"
#include <stdio.h>

char *str_copy(char *s)
{
	int l = strlen(s) + 1;
	char *ns;
	if((ns = malloc(l * sizeof(char))) == NULL)
		return NULL;
	char *p = ns;
	while(*s != '\0')
		*p++ = *s++;
	*p = '\0';

	return ns;
}