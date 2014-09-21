/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Returns a pointer to the first occurence of char c in the string s
   or NULL if it isn't found. */ 
char *find_ptr(char *s, char c)
{
	char *i = s;
	while(tolower(*i) != tolower(c))
		if(*i++ == '\0')
			return NULL;

	return i;
}