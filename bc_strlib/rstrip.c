/* File: rstrip.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Removes the trailing whitespace from string s */
void rstrip(char *s)
{
	char *ps = s + strlen(s) - 1;
	while(*ps == ' ')
		ps--;
	ps++;
	*ps = '\0';
}