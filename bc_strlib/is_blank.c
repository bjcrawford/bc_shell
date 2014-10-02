/* File: is_blank.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Returns 1 is s is NULL, consists only of the null character ('\0')
   or only whitespace. Returns 0 otherwise. */
int is_blank(char *s)
{
	if(s == NULL || *s == '\0')
		return 1;

	char *i = s;
	while(*i != '\0')
		if(*i++ != ' ')
			return 0;

	return 1;
}