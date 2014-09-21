/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Removes the leading and trailing whitespace from string s */
void strip(char *s)
{
	lstrip(s);
	rstrip(s);
}