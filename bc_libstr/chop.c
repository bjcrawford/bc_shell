/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <stdlib.h>

char **chop(char *s, char c)
{
	char pat[2];
	pat[0] = c;
	pat[1] = '\0';
	int l = count_occurences(s, pat) + 2;
	char **slices;
	if((slices = malloc(sizeof(char*) * l)) == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		return NULL;
	}
	char *psb = s;
	char *pse = s;
	int i;
	for(i = 0; i < l - 1; i++)
	{
		while(*pse != c)
			pse++;
		int wl = pse - psb;
		if((slices[i] = malloc(sizeof(char) * wl)) == NULL)
		{
			fprintf(stderr, "Error allocating memory\n");
			return NULL;
		} 
		char *pslice = slices[i];
		while(*psb != c)
			*pslice++ = *psb++;
		*pslice = '\0';
		psb += 1;
		pse = psb;
	}
	slices[i] = NULL;

	return slices;
}