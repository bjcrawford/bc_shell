/* File: lstrip.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Removes the leading whitespace from string s */
void lstrip(char *s)
{
	char *ps = s;
	while(*ps == ' ')
		ps++;
	while((*s = *ps) != '\0')
	{
		ps++;
		s++;
	}
}