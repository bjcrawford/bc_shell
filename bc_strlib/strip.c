/* File: strip.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Removes the leading and trailing whitespace from string s */
void strip(char *s)
{
	lstrip(s);
	rstrip(s);
}