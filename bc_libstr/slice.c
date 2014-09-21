/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <stdlib.h>

char *slice(char *s, int b, int e)
{
	if(e > strlen_bc(s) || b > strlen_bc(s)-1 || 
	   e < b || e < 0 || b < 0 || s == NULL)
		return NULL;
	int l = e - b + 2;
	char *ns;
	if((ns = malloc(sizeof(char) * l)) == NULL)
	{
		fprintf(stderr, "Error allocating memory.\n");
		return NULL;
	}
	char *ps = s;
	char *pns = ns;
	int i ;
	for(i = 0; i < b; i++)
		ps++;
	for(i = b; i <= e; i++)
		*pns++ = *ps++;
	*pns = '\0';

	return ns;
}