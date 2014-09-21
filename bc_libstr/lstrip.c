/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Removes the leading whitespace from string s */
void lstrip(char *s)
{
	char *ps = s;
	while(*ps == ' ')
		ps++;
	while((*s = *ps) != '\0')
	{
		ps++;
		s++;
	}
}