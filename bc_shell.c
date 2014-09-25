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

#include "bc_strlib/bc_strlib.h"

#define FALSE 0
#define TRUE 1

/* Flags */
#define DEBUG 1

/* Error codes */
#define EXIT_SUCCESS 0
#define ALLOC_ERROR 1
#define PARSE_ENV_VAR_ERROR 2

/* Constants */
#define BUFF_SIZE 300     /* Input buffer size */

/* External variables */
extern char **environ;

/* Function declarations */
int shell_prompt();
int parse_env_var(char***);
int read_and_space_input(char**);
int parse_input(char*, int*, int***, char****);
int parse_command(char*, int**, char***);
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
	int i;             /* Iteration variable */
	int num_commands;  /* Number of commands in input */
	int **num_args;    /* Array of num of args for each command */
	int i_argc;        /* Num of args in input */
	int pid;           /* Child process id */
	int ce_flag = 0;   /* Concurrent execution flag */
	char *input_string;
	char ***commands;
	char *command;     /* Command name */
	char **paths;      /* Array of paths contained in $PATH */
	char **i_argv;     /* Array of input, i_argv[0]: command, i_argv[1-N]: arguments */

	if(!parse_env_var(&paths))
		return parse_env_var_error();

	//while(parse_input(&i_argc, &i_argv, &ce_flag))
	while(read_and_space_input(&input_string))
	{

		printf("input_string: %s\n", input_string);

		parse_input(input_string, &num_commands, &num_args, &commands);

		printf("input_string: %s\n", input_string);
		/*
		if(DEBUG)
		{
			printf("i_argc: %d\n", i_argc);
			i = 0;
			while(i_argv[i] != NULL)
			{
				printf("i_argv[%d]: %s\n", i, i_argv[i]);
				i++;
			}
		}

		
		printf("ce_flag: %d\n", ce_flag);
		if(check_existence(i_argv[0], paths))
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
		
		ce_flag = 0;
		for(i = 0; i_argv[i] != NULL; i++)
			free(i_argv[i]);
		free(i_argv);
		*/
	}

	/*
	for(i = 0; i_argv[i] != NULL; i++)
		free(i_argv[i]);
	free(i_argv);
	free(paths);
	*/

	printf("bc_shell stopped.\n");

	return EXIT_SUCCESS;
}

int parse_env_var(char ***paths)
{
	int i = 0;
	char *path;

	while(environ[i] != NULL && !prefixcmp_igncase(environ[i], "path="))
		i++;
	path = replace(environ[i], "PATH=", "");
	*paths = chop(path, ':');

	free(path);

	if(*paths == NULL)
		return 0;
	else
		return 1;
}

int read_and_space_input(char **input_string)
{
	int i;
	int c;             /* Input character storage */
	int t;             /* Temporary character storage */
	char buffer[BUFF_SIZE + 1];

	printf("bc_shell-> ");
	for(i = 0; (c = getchar()) != EOF && c != '\n' && i < BUFF_SIZE; i++)
	{
		if(c == '<' || c == '>' || c == '|' || c == '&')
			if(i != 0 && buffer[i-1] != ' ')
				buffer[i++] = ' ';

		if(i != 0 && c != ' ')
		{
			t = buffer[i-1];
			if(t == '<' || t == '>' || t == '|' || t == '&')
				buffer[i++] = ' ';
		}
		buffer[i] = c;
	}
	buffer[i] = '\0';

	*input_string = str_copy(buffer);

	if(prefixcmp_igncase("exit", buffer))
		return 0;
	else
		return 1;
}

int parse_input(char *input_string, int *num_commands, int ***num_args, char ****commands)
{
	int i = 0, j = 0;
	char **command_strings = chop(input_string, '|');

	for(i = 0; command_strings[i] != NULL; i++);
	*num_commands = i;

	if((num_args = malloc(*num_commands * sizeof(int*))) == NULL)
		return allocation_error("num_args");

	if((commands = malloc(*num_commands * sizeof(char**))) == NULL)
		return allocation_error("commands");

	for(i = 0; i < *num_commands; i++)
	{
		strip(command_strings[i]);
		if(DEBUG)
		{
			printf("command_string[%d]: %s\n", i, command_strings[i]);
		}
		parse_command(command_strings[i], &num_args[i], commands[i]);
	}

	for(i = 0; command_strings[i] != NULL; i++)
		free(command_strings[i]);
	free(command_strings);
}

int parse_command(char *command_string, int **num_args, char ***args)
{
	int i = 0;
	args = chop(command_string, ' ');
	for(i = 0; args[i] != NULL; i++)
		if(DEBUG)
			printf("  args[%d]: %s\n", i, args[i]);
	*num_args = i;
}

int parse_input_old(int *i_argc, char ***i_argv, int *ce_flag)
{
	int i;
	int c;             /* Input character storage */
	int t;             /* Temporary character storage */
	char buffer[BUFF_SIZE + 1];
	char **dp;

	printf("bc_shell-> ");
	for(i = 0; (c = getchar()) != EOF && c != '\n' && i < BUFF_SIZE; i++)
	{
		if(c == '<' || c == '>' || c == '|' || c == '&')
			if(i != 0 && buffer[i-1] != ' ')
				buffer[i++] = ' ';

		if(i != 0 && c != ' ')
		{
			t = buffer[i-1];
			if(t == '<' || t == '>' || t == '|' || t == '&')
				buffer[i++] = ' ';
		}
		buffer[i] = c;
	}
	buffer[i] = '\0';

	if(DEBUG)
		printf("Spaced Input: %s\n", buffer);

	*i_argv = chop(buffer, ' ');
	dp = *i_argv;

	while(*dp != NULL)
		dp++;

	*i_argc = dp - *i_argv;

	if(strcmp_igncase((*i_argv)[*i_argc-1], "&") == 0)
	{
		*ce_flag = 1;
		free((*i_argv)[*i_argc-1]);
		(*i_argv)[*i_argc-1] = NULL;
		*i_argc = *i_argc - 1;
	}

	if(i_argc == 0 || strcmp_igncase("exit", (*i_argv)[0]) == 0)
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
	return prefixcmp_igncase(s, "/")  ||
	       prefixcmp_igncase(s, "./") ||
	       prefixcmp_igncase(s, "../");
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
