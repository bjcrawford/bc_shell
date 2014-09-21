#include <stdio.h>

#ifndef BC_LIBSTR
#define BC_LIBSTR

int    begins_with_ignore_case(char*, char*);
char  *center(char*, int);
char **chop(char*, char);
int    count_occurences(char*, char*);
char  *ellipsize(char*, int);
int    ends_with_ignore_case(char*, char*);
int    find_idx(char*, char);
char  *find_ptr(char*, char);
int    index_of_first_difference(char*, char*);
int    is_blank(char*);
void   lstrip(char*);
char  *make_space(int);
char  *remove_chars(char*, char*);
char  *repeat(char*, int);
char  *replace(char*, char*, char*);
void   rstrip(char*);
char  *slice(char*, int, int);
int    strcmp_ign_case(char*, char*);
void   strip(char*);
int    strlen_bc(char*);
int    word_count(char*);

#endif