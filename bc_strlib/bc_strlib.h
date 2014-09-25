/* File: bc_strlib.h
   Author: Brett Crawford
   Date: 2014-03-25
   Description: This file is part of the bc_strlib */

#include <stdio.h>
#include <string.h>

#ifndef BC_STRLIB
#define BC_STRLIB

char  *append(char*, char*);
int    prefixcmp_igncase(char*, char*);
char **chop(char*, char);
int    count_occurences(char*, char*);
int    suffixcmp_igncase(char*, char*);
char  *replace(char*, char*, char*);
char  *str_copy(char*);
int    strcmp_igncase(char*, char*);

#endif