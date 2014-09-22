/* File: bc_shell.c
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:  */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bc_libstr/bc_libstr.h"

#define FALSE 0
#define TRUE 1

/* Error codes */
#define EXIT_SUCCESS 0
#define ALLOC_ERROR 1
#define PATH_PARSE_ERROR 2

/* External variables */
extern char **environ;

/* Function declarations */
int shell_prompt();
int parse_path(char***);
int parse_input(int*, char***);
int check_existence(char*, char**, char**);
int allocation_error(char*);
int path_parse_error();

int main(int argc, char **argv)
{
	return shell_prompt();
}

int shell_prompt()
{
	int i;
	int i_argc;
	int found;
	char *command;
	char **paths;
	char **i_argv;

	if(!parse_path(&paths))
		return path_parse_error();

	printf("Paths found in $PATH:\n");
	for(i = 0; paths[i] != NULL; i++)
		printf("paths[%d]: %s\n", i, paths[i]);

	while(parse_input(&i_argc, &i_argv))
	{
		found = check_existence(i_argv[0], &command, paths);

		if(found)
		{
			printf("Found command: %s located at %s\n", i_argv[0], command);
		}
		else
			printf("-bc_shell: %s: command not found\n", i_argv[0]);
		
		free(command);
		for(i = 0; i_argv[i] != NULL; i++)
			free(i_argv[i]);
		free(i_argv);
	}

	for(i = 0; i_argv[i] != NULL; i++)
		free(i_argv[i]);
	free(i_argv);
	free(paths);

	printf("Exiting...\n");

	return EXIT_SUCCESS;
}

int parse_path(char ***paths)
{
	int i = 0;
	char *path;

	while(environ[i] != NULL && !begins_with_ignore_case(environ[i], "path="))
		i++;
	path = replace(environ[i], "PATH=", "");
	*paths = chop(path, ':');

	free(path);

	if(*paths == NULL)
		return 0;
	else
		return 1;
}

int parse_input(int *i_argc, char ***i_argv)
{
	int i;
	int c;        /* Input character storage */
	char buffer[201];
	char **dp;

	printf("bc_shell-> ");
	for(i = 0; (c = getchar()) != EOF && c != '\n' && i < 200; i++)
		buffer[i] = c;
	buffer[i] = '\0';

	*i_argv = chop(buffer, ' ');
	dp = *i_argv;

	while(*dp != NULL)
		dp++;

	*i_argc = dp - *i_argv;

	if(i_argc == 0 || strcmp_ign_case("exit", *i_argv[0]) == 0)
		return 0;
	else
		return 1;
}

int check_existence(char *argv0, char **command, char **paths)
{
	int i;
	int result;
	char *temp;
	if(begins_with_ignore_case(argv0, "/")  ||
	   begins_with_ignore_case(argv0, "./") ||
	   begins_with_ignore_case(argv0, "../") )
	{
		if(access(argv0, F_OK) != -1)
		{
			*command = str_copy(argv0);
			result = 1;
		}
	}
	else
	{
		for(i = 0; paths[i] != NULL; i++)
		{
			temp = append("/", argv0);
			*command = append(paths[i], temp);
			if(access(*command, F_OK) != -1)
			{
				result = 1;
				free(temp);
				break;
			}
		}
	}

	return result;
}

/* Reports an allocation error
   Input: character pointer, name of the allocation attempted
   Output: integer, error code for allocation error */
int allocation_error(char *name)
{
	fprintf(stderr, "Error allocating for %s\n", name);
	fprintf(stderr, "Press enter to exit.\n");
	getchar();

	return ALLOC_ERROR;
}

/* Reports a path parsing error
   Output: integer, error code for allocation error */
int path_parse_error()
{
	fprintf(stderr, "Error parse the path environment variable\n");
	fprintf(stderr, "Press enter to exit.\n");
	getchar();

	return PATH_PARSE_ERROR;
}
