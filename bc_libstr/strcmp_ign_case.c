/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <ctype.h>

/* Compares strings s1 and s2 ignoring case. Returns a negative
   number if s1 is lexicographically less than s2, a positive 
   number if s1 is lexicographically greater than s2, and zero
   if the two strings are equivalent */ 
int strcmp_ign_case(char *s1, char*s2)
{
	char *ps1 = s1;
	char *ps2 = s2;
	int diff = 0;
	while(*ps1 != '\0' && *ps2 != '\0' && diff == 0)
		diff = tolower(*ps1++) - tolower(*ps2++);
	if(diff == 0)
		diff = tolower(*ps1) - tolower(*ps2);

	return diff;
}