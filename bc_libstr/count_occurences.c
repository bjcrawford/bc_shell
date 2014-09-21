/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Returns the numbers of occurrences of string pat in string s */
int count_occurences(char *s, char *pat)
{
	int i;
	int c = 0;
	int lpat = strlen_bc(pat);
	char *ps = s;
	char *ppat = pat;
	while(*ps != '\0')
	{
		int match = 1;
		for(i = 0; i < lpat && match == 1; i++)
		{
			if(*ps == '\0' || *ps != *ppat)
				match = 0;
			else
				ppat++;
			ps++;
		}
		if(match == 1)
			c++;
		ps -= i - 1;
		ppat = pat;
	}

	return c;
}
