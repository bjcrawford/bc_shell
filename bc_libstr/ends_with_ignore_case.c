/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Returns 1 if suff is a suffix of s, ignoring case. Otherwise 0 */
int ends_with_ignore_case(char *s, char *suff)
{
	int sl = strlen_bc(s);
	int suffl = strlen_bc(suff);
	char *ps = s-1;
	char *psuff = suff-1;
	ps += sl;
	psuff += suffl;
	while(sl-- && suffl--)
		if(tolower(*ps--) != tolower(*psuff--))
			return 0;

	return 1;
}