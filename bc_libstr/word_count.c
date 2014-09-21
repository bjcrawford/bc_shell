/* Brett Crawford
   Prof Fiore, TA Liang
   CIS2107 - Sec 1 */

#include "bc_libstr.h"

/* Returns the number of words (separated by whitespace)
   in string s */
int word_count(char *s)
{
	char *ps = s;
	int wc = 0;
	int state = 0;
	while(*ps != '\0')
	{
		if(state == 1)
		{
			if(isspace(*ps))
				state = 0;
		}
		else
		{
			if(!isspace(*ps))
			{
				wc++;
				state = 1;
			}
		}
		ps++;
	}

	return wc;
}