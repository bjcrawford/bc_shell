/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <stdlib.h>

/* Returns a new string which consists of n spaces. It is up to the
   caller to free the memory allocated by make_space(). */
char *make_space(int n)
{
	char *s;
	if((s = malloc(sizeof(char) * (n+1))) == NULL)
	{
		printf("Error allocating memory\n");
		return NULL;
	}
	int i;
	char *p = s;
	while(p-s < n)
		*p++ = ' ';
	*p = '\0';

	return s;
}