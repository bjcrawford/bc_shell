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

#define DEBUG 0

/* Error codes */
#define EXIT_SUCCESS 0
#define ALLOC_ERROR 1
#define PARSE_ENV_VAR_ERROR 2

/* External variables */
extern char **environ;

/* Function declarations */
int shell_prompt();
int parse_env_var(char***);
int parse_input(int*, char***);
int check_existence(char*, char**);
int abs_rel_check(char*);
int allocation_error(char*);
int parse_env_var_error();

int main(int argc, char **argv)
{
	return shell_prompt();
}

int shell_prompt()
{
	int i;
	int i_argc;
	int found;
	int pid;
	char *command;
	char **paths;
	char **i_argv;

	if(!parse_env_var(&paths))
		return parse_env_var_error();

	while(parse_input(&i_argc, &i_argv))
	{
		found = check_existence(i_argv[0], paths);

		if(found)
		{
			command = str_copy(i_argv[0]);

			pid = fork();
			if(pid == 0)
			{
				execvp(command, i_argv);
			}
			else
			{
				wait(pid);
			}

			free(command);
		}
		
		for(i = 0; i_argv[i] != NULL; i++)
			free(i_argv[i]);
		free(i_argv);
	}

	for(i = 0; i_argv[i] != NULL; i++)
		free(i_argv[i]);
	free(i_argv);
	free(paths);

	printf("bc_shell stopped.\n");

	return EXIT_SUCCESS;
}

int parse_env_var(char ***paths)
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
	int c;             /* Input character storage */
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

int check_existence(char *argv0, char **paths)
{
	int i;
	int result = 0;
	char *temp;
	char *path = NULL;

	if(abs_rel_check(argv0))
	{
		if(access(argv0, F_OK) != -1)
			result = 1;
	}
	else
	{
		for(i = 0; paths[i] != NULL; i++)
		{
			temp = append("/", argv0);
			path = append(paths[i], temp);
			free(temp);
			if(access(path, F_OK) != -1)
			{
				result = 1;
				break;
			}
			free(path);
		}
	}

	if(result)
	{
		if(path == NULL)
		{
			if(DEBUG)
				printf("Found command: %s\n", argv0);
		}
		else
		{
			if(DEBUG)
				printf("Found command: %s located at %s\n", argv0, path);
			free(path);
		}
	}
	else
		printf("-bc_shell: %s: command not found\n", argv0);

	return result;
}

int abs_rel_check(char *s)
{
	return begins_with_ignore_case(s, "/")  ||
	       begins_with_ignore_case(s, "./") ||
	       begins_with_ignore_case(s, "../");
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
int parse_env_var_error()
{
	fprintf(stderr, "Error parsing environment variables\n");
	fprintf(stderr, "Press enter to exit.\n");
	getchar();

	return PARSE_ENV_VAR_ERROR;
}
