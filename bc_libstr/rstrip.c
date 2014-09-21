/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Removes the trailing whitespace from string s */
void rstrip(char *s)
{
	char *ps = s + strlen_bc(s) - 1;
	while(*ps == ' ')
		ps--;
	ps++;
	*ps = '\0';
}