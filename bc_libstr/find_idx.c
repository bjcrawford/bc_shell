/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

   #include "bc_libstr.h"

/* Returns the index of the first occurence of char c in the string s
   or -1 if it isn't found. */
int find_idx(char *s, char c)
{
	char *i = s;
	while(tolower(*i) != tolower(c))
		if(*i++ == '\0')
			return -1;

	return i - s;
}