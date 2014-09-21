/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"
#include <stdlib.h>

/* Returns a new string which consists of the characters of s centered
   in a string that is at least n characters wide. */
char *center(char *s, int n)
{
	char *ns;
	int l = strlen_bc(s);
	int w = (l > n) ? l+1 : n+1;
	if((ns = malloc(sizeof(char) * w)) == NULL)
	{
		printf("Error allocating memory\n");
		return NULL;
	}
	char *i = ns;
	int diff = (n > l) ? (n-l) : 0;
	int b = diff/2;
	int e = (diff%2 == 1) ? b+1 : b;
	while(b--)
		*i++ = ' ';
	int c = 0;
	while(c++ < l)
		*i++ = *s++;
	while(e--)
		*i++ = ' ';
	*i = '\0';

	return ns;
}