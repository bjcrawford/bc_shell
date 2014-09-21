/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <stdlib.h>
#include <ctype.h>

/* Returns a new string consisting of the first n words
   followed by "...". If there are fewer than n words in
   string s, a copy of s is returned. */
char *ellipsize(char *s, int n)
{
	if(n < 0 || s == NULL)
		return NULL;
	char *ns;
	char *ps = s;
	char *pns;
	int l;
	int sw = word_count(s);

	if(sw < n) /* return a copy of s */
		l = strlen_bc(s) + 1;
	else /* otherwise, determine the length of the new string */
	{
		int wc = 0;
		while(*ps != '\0' && wc < n)
		{
			while(isspace(*ps))
				ps++;
			if(*ps != '\0')
				wc++;
			while(*ps != '\0' && !isspace(*ps))
				ps++;
		}
		l = ps - s + 4;
	}

	if((ns = malloc(sizeof(char) * l)) == NULL)
	{
		fprintf(stderr, "Error allocationg memory");
		return NULL;
	}
	pns = ns;

	if(sw >= n) 
		l -= 3; /* leave room for the ellipses */
	if(n == 0)
		l -= 1;

	/* copy the original to the new */
	ps = s;
	while(ps - s < l)
		*pns++ = *ps++;

	if(sw >= n)
	{
		*pns++ = '.';
		*pns++ = '.';
		*pns++ = '.';
	}
	*pns = '\0';

	return ns;
}