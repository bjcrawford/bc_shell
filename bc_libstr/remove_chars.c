/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <stdlib.h>

/* Returns a new string consisting of the string s1 with all of the
   characters in string s2 removed */
char *remove_chars(char *s1, char *s2)
{
	char *ps1;
	char *ps2;
	int nc = 0;
	for(ps2 = s2; *ps2 != '\0'; ps2++)
		for(ps1 = s1; *ps1 != '\0'; ps1++)
			if(*ps2 == *ps1)
				nc++;
	int nlen = strlen_bc(s1) - nc + 1;
	char *ns;
	if((ns = malloc(sizeof(char) * nlen)) == NULL)
	{
		printf("Error allocating memory\n");
		return NULL;
	}
	char *pns = ns;
	int found;
	for(ps1 = s1; *ps1 != '\0'; ps1++)
	{
		found = 0;
		for(ps2 = s2; !found && *ps2 != '\0'; ps2++)
			if(*ps1 == *ps2)
				found = 1;
		if(!found)
			*pns++ = *ps1;
	}
	*pns = '\0';

	return ns;	
}