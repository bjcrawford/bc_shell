/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <ctype.h>

/* Compares strings s1 and s2 and returns the index at which the
   two strings begin to differ. If the strings are equal, the
   functions returns -1. */
int index_of_first_difference(char *s1, char *s2)
{
	int i = 0;
	char *ps1 = s1;
	char *ps2 = s2;
	while(*ps1 != '\0' && *ps2 != '\0')
	{
		if(tolower(*ps1) != tolower(*ps2))
			return i;
		i++;
		ps1++;
		ps2++;
	}
	if(tolower(*ps1) != tolower(*ps2))
		return i;
	else
		return -1;
}