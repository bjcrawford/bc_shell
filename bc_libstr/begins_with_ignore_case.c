/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Returns 1 if pre is a prefix of s, ignoring case. Otherwise 0 */
int begins_with_ignore_case(char *s, char *pre)
{
	int sl = strlen_bc(s);
	int pl = strlen_bc(pre);
	while(sl-- && pl--)
		if(tolower(*s++) != tolower(*pre++))
			return 0;

	return 1;
}