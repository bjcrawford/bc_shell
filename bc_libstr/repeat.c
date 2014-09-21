/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <stdlib.h>

/* Returns a new string consisting of the characters in string
   s repeated n times. If s is NULL, NULL is returned */
char *repeat(char *s, int n)
{
	if(s == NULL || n < 0)
		return NULL;
	char *ns;
	int l = strlen_bc(s) * n + 1;
	if((ns = malloc(sizeof(char) * l)) == NULL)
	{
		fprintf(stderr, "Error allocating memory.\n");
		return NULL;
	}
	int i;
	char *ps = s;
	char *pns = ns;
	for(i = 0; i < n; i++)
	{
		while(*ps != '\0')
			*pns++ = *ps++;
		ps = s;
	}
	*++pns = '\0';

	return ns;
}