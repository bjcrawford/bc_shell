/* File: prefix_igncase.c
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include "bc_strlib.h"

/* Returns 1 if pre is a prefix of s, ignoring case. Otherwise 0 */
int prefixcmp_igncase(char *s, char *pre)
{
	int sl = strlen(s);
	int pl = strlen(pre);
	while(sl-- && pl--)
		if(tolower(*s++) != tolower(*pre++))
			return 0;

	return 1;
}
