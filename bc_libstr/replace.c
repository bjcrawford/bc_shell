/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <stdlib.h>

/* Returns a copy of the string s, but with each instance of
   string pat replaced with string rep. */
char *replace(char *s, char *pat, char *rep)
{
	if(s == NULL)
		return NULL;
	int ls = strlen_bc(s);
	int lpat = strlen_bc(pat);
	int lrep = strlen_bc(rep);
	int o = count_occurences(s, pat);
	int lns = ls - (o*lpat) + (o*lrep) + 1;
	char *ns;
	if((ns = malloc(sizeof(char) * lns)) == NULL)
	{
		fprintf(stderr, "Error allocating memory");
		return NULL;
	}

	char *ps = s;
	char *pns = ns;
	char *ppat = pat;
	char *prep = rep;
	int i;

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
		{
			prep = rep;
			for(i = 0; i < lrep; i++)
				*pns++ = *prep++;
		}
		else
		{
			ps -= i;
			*pns++ = *ps;
			ps += i;
			ps -= i - 1;
		}
		ppat = pat;
	}
	*pns = '\0';

	return ns;
}