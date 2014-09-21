/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

int strlen_bc(char *s)
{
	char *i = s;
	while(*i++ != '\0');

	return (i-s)-1;
}