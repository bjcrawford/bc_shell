/* File: suffixcmp_igncase.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Returns 1 if suff is a suffix of s, ignoring case. Otherwise 0 */
int suffixcmp_igncase(char *s, char *suff)
{
	int sl = strlen(s);
	int suffl = strlen(suff);
	char *ps = s-1;
	char *psuff = suff-1;
	ps += sl;
	psuff += suffl;
	while(sl-- && suffl--)
		if(tolower(*ps--) != tolower(*psuff--))
			return 0;

	return 1;
}