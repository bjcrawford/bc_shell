/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Returns 1 is s is NULL, consists only of the null character ('\0')
   or only whitespace. Returns 0 otherwise. */
int is_blank(char *s)
{
	if(s == NULL || *s == '\0')
		return 1;

	char *i = s;
	while(*i != '\0')
		if(*i++ != ' ')
			return 0;

	return 1;
}