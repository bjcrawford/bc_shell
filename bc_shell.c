/* File: bc_shell.c
   Author: Brett Crawford
   Date: 2014-
   Prof: Kwatny
   TAs: Liang and Casey
   Course: CIS 3207, Sec 2
   Description:  */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
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
#define ALLOC_ERROR -1
#define PARSE_ENV_VAR_ERROR -2

/* Constants */
#define BUFF_SIZE 300     /* Input buffer size */

/* External variables */
extern char **environ;

/* Function declarations */
int shell_prompt();
int parse_env_var(char***);
int read_and_space_input(char**);
int parse_input(char*, int*, int**, char****);
int parse_command(char*, int*, char***);
int check_commands(int, char***, char**);
int check_existence(char*, char**);
int allocate_flags_memory(int**, int **, int**, int**, int);
int allocate_redirection_memory(char***, char***, int);
int allocate_pipe_memory(int***, int);
int abs_rel_check(char*);
int allocation_error(char*);
int parse_env_var_error();

int main(int argc, char **argv)
{
	return shell_prompt();
}

int shell_prompt()
{
	pid_t child_h_pid;     /* Child handler process id */
	pid_t child_i_pid;     /* Child instance process id */

	int i, j, k;           /* Iteration variables */
	int num_commands;      /* Number of commands in input */
	int or_fd;             /* Output redirection file descriptor */
	int ir_fd;             /* Input redirection file descriptor */
	int pi_error;          /* parse_input() error code, if any */
	int afm_error;         /* allocate_flags_memory() error code, if any */
	int arm_error;         /* allocate_redirection_memory() error code, if any */
	int apm_error;         /* allocate_pipe_memory() error code, if any */
	int *num_args;         /* Array of num of args for each command */
	int *ce_flags;         /* Array of concurrent execution flags */
	int *or_flags;         /* Array of output redirection flags */
	int *aor_flags;        /* Array of appended output redirection flags */
	int *ir_flags;         /* Array of input redirection flags */
	int **pc_fd;           /* Array to hold previous command to 
	                          current command file descriptors */

	char *input_string;    /* String to hold user input */
	char **out_redir;      /* Array of output redirection strings */
	char **in_redir;       /* Array of input redirection strings */ 
	char **paths;          /* Array of paths contained in $PATH */
	char ***commands;      /* Array of commands,
	                          each command is an array of args, */

	if(!parse_env_var(&paths))
		return parse_env_var_error();

	while(read_and_space_input(&input_string))
	{	
		/* If input is blank, return to top to while loop */	
		if(is_blank(input_string))
			continue;

		pi_error = parse_input(input_string, &num_commands, &num_args, &commands);
		if(pi_error != EXIT_SUCCESS)
			return pi_error;

		if(DEBUG)
		{
			printf("input_string: %s\n", input_string);
			printf("num_commands: %d\n", num_commands);
			for(i = 0; i < num_commands; i++)
			{
				printf("num_args[%d]: %d\n", i, num_args[i]);
				for(j = 0; j < num_args[i]; j++)
				{
					printf("commands[%d][%d]: %s\n", i, j, commands[i][j]);
				}
			}
		}

		

		if(check_commands(num_commands, commands, paths)) /* If all commands are valid */
		{
			/* Allocate memory */
			afm_error = allocate_flags_memory(&ce_flags, &or_flags, &aor_flags, &ir_flags, num_commands);
			if(afm_error != EXIT_SUCCESS)
				return afm_error;

			arm_error = allocate_redirection_memory(&out_redir, &in_redir, num_commands);
			if(arm_error != EXIT_SUCCESS)
				return arm_error;

			apm_error = allocate_pipe_memory(&pc_fd, num_commands);
			if(apm_error != EXIT_SUCCESS)
				return apm_error;

			/* Check each command for operators */
			for(i = 0; i < num_commands; i++)
			{
				/* Check for concurrent execution operator */
				if(strcmp_igncase(commands[i][num_args[i]-1], "&") == 0)
				{
					ce_flags[i] = 1;
					commands[i][num_args[i]-1] = NULL;
					num_args[i]--;
				}

				/* Check for output redirection operator */
				for(j = 0; j < num_args[i]; j++)
				{
					if(strcmp_igncase(commands[i][j], ">") == 0 || strcmp_igncase(commands[i][j], ">>") == 0)
					{
						if(i == num_commands - 1 && j < num_args[i] - 1)
						{
							if(strcmp_igncase(commands[i][j], ">") == 0)
								or_flags[i] = 1;
							else
								aor_flags[i]= 1;
							if((out_redir[i] = str_copy(commands[i][j+1])) == NULL)
								return allocation_error("bc_shell.c: shell_prompt(): alloc error for out_redir[i]");
						}
						for(k = j; k < num_args[i] - 1; k++)
							commands[i][k] = commands[i][k+2];
						num_args[i] = num_args[i] - 2;
					}
				}

				/* Check for input redirection operator */
				for(j = 0; j < num_args[i]; j++)
				{
					if(strcmp_igncase(commands[i][j], "<") == 0)
					{
						if(i == 0 && j < num_args[i] - 1)
						{
							ir_flags[i] = 1;
							if((in_redir[i] = str_copy(commands[i][j+1])) == NULL)
								return allocation_error("bc_shell.c: shell_prompt(): alloc error for in_redir[i]");
						}
						for(k = j; k < num_args[i] - 1; k++)
							commands[i][k] = commands[i][k+2];
						num_args[i] = num_args[i] - 2;
					}
				}
			}
			
			if(DEBUG)
			{
				for(i = 0; i < num_commands; i++)
				{
					printf("ce_flags[%d]: %d\n", i, ce_flags[i]);
					printf("or_flags[%d]: %d\n", i, or_flags[i]);
					printf("aor_flags[%d]: %d\n", i, aor_flags[i]);
					if(or_flags[i] || aor_flags[i])
						printf("out_redir[%d]: %s\n", i, out_redir[i]);
					printf("ir_flags[%d]: %d\n", i, ir_flags[i]);
					if(ir_flags[i])
						printf("in_redir[%d]: %s\n", i, in_redir[i]);
				}

				printf("input_string: %s\n", input_string);
				printf("num_commands: %d\n", num_commands);
				for(i = 0; i < num_commands; i++)
				{
					printf("num_args[%d]: %d\n", i, num_args[i]);
					for(j = 0; j < num_args[i]; j++)
					{
						printf("commands[%d][%d]: %s\n", i, j, commands[i][j]);
					}
				}
			}

			/* Set up a previous cmd to current cmd pipe for each command */
			for(i = 0; i < num_commands; i++)
			{
				if(pipe(pc_fd[i]) == -1)
					printf("Error initializing pc_fd[%d]\n", i);
			}

			/* Create a child to handle creating the children for command(s) */
			child_h_pid = fork(); 
			if(child_h_pid == 0)
			{
				for(i = 0; i < num_commands; i++)
				{
					/* Create a child for each command */
					child_i_pid = fork();
					if(child_i_pid == 0)
					{
						if(i > 0) /* If not the first of the "piped" together commands */
						{
							/* Replace stdin with read side of pipe */
							if(dup2(pc_fd[i][0], fileno(stdin)) == -1)
								printf("dup2 error replacing stdin with pc_fd\n");
						}
						else  /* If the first of the "piped" together commands */
						{
							if(ir_flags[i]) /* If input redirection flag is set */
							{
								/* Open input redirection and use to replace stdin */
								ir_fd = open(in_redir[i], O_RDONLY);
								if(dup2(ir_fd, fileno(stdin)) == -1)
									printf("dup2 error replacing stdin with ir_fd\n");
							}
						}
						if(i < num_commands - 1) /* If not the last of the "piped" together commands */
						{
							/* Replace stdout with the write side of pipe */
							if(dup2(pc_fd[i+1][1], fileno(stdout)) == -1)
								printf("dup2 error replacing stdout with pc_fd\n");
						}
						else /* If the last of the "piped" together commands */
						{
							if(or_flags[i] || aor_flags[i]) /* If the output redirection flag is set */
							{
								/* Open output redirection and use to replace stdout */
								if(or_flags[i])
									or_fd = open(out_redir[i], O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
								else
									or_fd = open(out_redir[i], O_WRONLY|O_APPEND|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
								if(dup2(or_fd, fileno(stdout)) == -1)
									printf("dup2 error replacing stdout with or_fd\n");
							}
						}

						/* Close all the child's file descriptors */
						for(j = 0; j < num_commands; j++)
						{
							close(pc_fd[j][0]);
							close(pc_fd[j][1]);
							if(or_flags[i] || aor_flags[i])
								close(or_fd);
							if(ir_flags[i])
								close(ir_fd);
						}
						execvp(commands[i][0], commands[i]);
					}
					else
					{
						/* Close the child handler's file descriptors for this child */
						close(pc_fd[i][0]);
						close(pc_fd[i][1]);

						if(!ce_flags[i])
							wait(child_i_pid);
					}
				}
				/* Child handler can exit after creating all children */
				return EXIT_SUCCESS;
			}
			else
			{
				/* Close the parent's file descriptors */
				for(i = 0; i < num_commands; i++)
				{
					close(pc_fd[i][0]);
					close(pc_fd[i][1]);
				}
				if(!ce_flags[num_commands-1])
					wait(child_h_pid);
			}
			
			/* Free all dynamically allocated memory */
			for(i = 0; i < num_commands; i++)
			{
				for(j = 0; j < num_args[i]; j++)
					free(commands[i][j]);
				free(commands[i]);
				free(out_redir[i]);
				free(in_redir[i]);
				free(pc_fd[i]);
			}
			free(commands);
			free(num_args);
			free(out_redir);
			free(in_redir);
			free(pc_fd);
			free(ce_flags);
			free(or_flags);
			free(aor_flags);
			free(ir_flags);		
		}
	}
	
	free(input_string);
	free(paths);
	printf("bc_shell stopped.\n");

	return EXIT_SUCCESS;
}

/* Parses $PATH from the environment variable into separate paths to be
   stored in an array of c strings.

   Input:  Address of a double pointer to char - Initally undefined, after the
           function it will contain an array of c strings of the paths in $PATH

   Output: Integer - Returns 1 if the paths were succesfully read and stored,
           otherwise returns error code. */
int parse_env_var(char ***paths)
{
	int i = 0;
	char *path;

	while(environ[i] != NULL && !prefixcmp_igncase(environ[i], "path="))
		i++;
	if((path = replace(environ[i], "PATH=", "")) == NULL)
		return allocation_error("bc_shell.c: parse_env_var(): alloc error for path");
	if((*paths = chop(path, ':')) == NULL)
		return allocation_error("bc_shell.c: parse_env_var(): alloc error for paths");

	free(path);

	return 1;
}

/* Reads input from stdin and stores the input into a c string. The input is
   modified by inserting whitespace between all operators and arguments. The 
   caller is responsible for freeing the dynamically allocated memory for
   input_string 

   Input:  Address of a pointer to char - Initally undefined, after the
           function it will contain a c string of the input.

   Output: Integer - Returns 1 if the input was successfully read and stored,
           returns 0 if the input == 'exit',
           otherwise returns error code. */
int read_and_space_input(char **input_string)
{
	int i;                        /* Iteration variable */
	int c;                        /* Input character storage */
	int t;                        /* Temporary character storage */
	char buffer[BUFF_SIZE + 1];   /* Input buffer */

	printf("bc_shell-> ");

	/* Get chars until EOF or newline is detected or until buffer size is exceeded */
	for(i = 0; (c = getchar()) != EOF && c != '\n' && i < BUFF_SIZE; i++)
	{
		/* If current char is operator and previous char is not 
		   whitespace, add whitespace before operator */
		if(c == '<' || c == '|' || c == '&')
		{
			if(i != 0 && buffer[i-1] != ' ')
			{
				buffer[i++] = ' ';
			}
		}
		/* If current char is '>' and previous char is not '>'
		   add whitespace before operator */
		else if(c == '>')
		{
			if(i != 0 && buffer[i-1] != '>' && buffer[i-1] != ' ')
			{
				buffer[i++] = ' ';
			}
		}


		/* If current char is not the first character of the input and 
		   is not whitespace and previous char is operator, add
		   whitespace after operator */
		if(i != 0 && c != ' ')
		{
			t = buffer[i-1];
			if(t == '<' || t == '|' || t == '&')
			{
				buffer[i++] = ' ';
			}
			else if(t == '>' && buffer[i-1] != '>')
			{
				buffer[i++] = ' ';
			}
		}

		/* Add current char to buffer */
		buffer[i] = c;
	}
	buffer[i] = '\0';

	/* Copy buffer into input string */
	if((*input_string = str_copy(buffer)) == NULL)
		return allocation_error("bc_shell.c: read_and_space_input(): alloc error for input_string");

	if(c == -1 || prefixcmp_igncase("exit", buffer))
		return 0;
	else
		return 1;
}

/* Parses a given string of commands separated by '|' into string of arguments
   separated by whitespace which is then parsed into an array of arguments.
   The caller is responsible for freeing the dynamically allocated
   memory for num_args and commands(commands[i][j], commands[i], and commands).

   Input:  C string - A string containing a multiple commands separated by "|"
           with arguments separated by whitespace.

           Address of an integer - Intially undefined, after the function
           it will contain the number of commands in the input string.

           Address of a pointer to integer - Initally undefined, after the
           function it will contain an array of ints, with each representing
           the number of arguments in each respective command.

           Address of a triple pointer to char - Initally undefined, after the function
           it will contain an array of commands with each element being an array of 
           c strings with each element being a separate argument from the given 
           command.

   Output: Integer - EXIT_SUCCESS if the command was parsed, otherwise error code */
int parse_input(char *input_string, int *num_commands, int **num_args, char ****commands)
{
	int i = 0;
	char **command_strings;

	if((command_strings = chop(input_string, '|')) == NULL)
		return allocation_error("bc_shell.c: parse_input(): alloc error for commands_strings");

	for(i = 0; command_strings[i] != NULL; i++);
	*num_commands = i;

	if((*num_args = calloc(*num_commands, sizeof(int))) == NULL)
		return allocation_error("bc_shell.c: parse_input(): alloc error for num_args");

	if((*commands = calloc(*num_commands, sizeof(char**))) == NULL)
		return allocation_error("bc_shell.c: parse_input(): alloc error for commands");

	for(i = 0; i < *num_commands; i++)
	{
		strip(command_strings[i]);
		parse_command(command_strings[i], &((*num_args)[i]), &((*commands)[i]));
	}

	for(i = 0; i < *num_commands; i++)
	{
		free(command_strings[i]);
	}
	free(command_strings);

	return EXIT_SUCCESS;
}

/* Parses a given string of arguments separated by whitespace into an array 
   of arguments. The caller is responsible for freeing the dynamically allocated
   memory in args(each args[i] and args itself)

   Input:  C string - A string containing a single command with arguments
           separated by whitespace.

           Address of an integer - Intially undefined, after the function
           it will contain the number of arguments in the command string.

           Address of a double pointer to char - Initally undefined,
           after the function it will contain an array of c strings
           with each element being a separate argument from the given 
           command string.

   Output: Integer - EXIT_SUCCESS if the command was parsed, otherwise error code */
int parse_command(char *command_string, int *num_args, char ***args)
{
	int i = 0;
	if((*args = chop(command_string, ' ')) == NULL)
		return allocation_error("bc_shell.c: parse_command(): alloc error for args");
	for(i = 0; (*args)[i] != NULL; i++);
	*num_args = i;

	return EXIT_SUCCESS;
}

/* Checks the given array of commands for the existence of each command 

   Input:  Integer - The number of commands in the second argument.

           Array of pointers to arrays of c strings - commands[0][0] would
           reference the first argument of the first command.

           Array of c strings - Contains the file paths to check.

   Output: Integer - 1 if all commands were located, otherwise 0. */
int check_commands(int num_commands, char ***commands, char **paths)
{
	int i;
	int result = 1;
	for(i = 0; i < num_commands && result; i++)
	{
		result = check_existence(commands[i][0], paths);
	}

	return result;
}

/* Checks the file paths contained in paths for the existence of the file name in argv0. 

   Input:  C string - Contains the name the file to check for existence.

           Array of c strings - Contains the file paths to check. 

   Output: Integer - 1 if the file was located, otherwise 0. */
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
			if((temp = append("/", argv0)) == NULL)
				return allocation_error("bc_shell.c: check_existence(): alloc error for temp");
			if((path = append(paths[i], temp)) == NULL)
				return allocation_error("bc_shell.c: check_existence(): alloc error for path");
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

/* Dynamically allocates memory for arrays of pointers to integers. This is used for
   allocating memory for the flags used to determine if a command uses '&', '<', or 
   '>' operators. The caller is responsible for freeing the dynamically allocated 
   memory for ce_flags, or_flags, and ir_flags.

   Input:  Address of a pointer to int - Initially undefined, after the function it will
           hold an array of zeroed integers used for concurrent execution flags.

           Address of a pointer to int - Initially undefined, after the function it will
           hold an array of zeroed integers used for output redirection flags.

           Address of a pointer to int - Initially undefined, after the function it will
           hold an array of zeroed integers used for input redirection flags.

           Integer - the number of integers to allocate space for.

   Output: Integer - EXIT_SUCCESS on successful allocation, otherwise error code. */
int allocate_flags_memory(int **ce_flags, int **or_flags, int **aor_flags, int **ir_flags, int num_commands)
{
	if((*ce_flags = calloc(num_commands, sizeof(int))) == NULL)
		return allocation_error("bc_shell.c: allocate_flags_memory(): alloc error for ce_flags");
	if((*or_flags = calloc(num_commands, sizeof(int))) == NULL)
		return allocation_error("bc_shell.c: allocate_flags_memory(): alloc error for or_flags");
	if((*aor_flags = calloc(num_commands, sizeof(int))) == NULL)
		return allocation_error("bc_shell.c: allocate_flags_memory(): alloc error for aor_flags");
	if((*ir_flags = calloc(num_commands, sizeof(int))) == NULL)
		return allocation_error("bc_shell.c: allocate_flags_memory(): alloc error for ir_flags");

	return EXIT_SUCCESS;
}

/* Dynamically allocates memory for arrays of pointers to c strings. This is used for
   allocating memory for the filenames of the output and input redirection destinations.
   The caller is responsible for freeing the dynamically allocated memory for out_redir 
   and in_redir.

   Input:  Address of a double pointer to char - Initially undefined, after the function
           it will hold the arrays containing the c strings of output redirection filenames.

           Address of a double pointer to char - Initially undefined, after the function
           it will hold the arrays containing the c strings of input redirection filenames.

           Integer - the number of pointers to c strings to allocate space for.

   Output: Integer - EXIT_SUCCESS on successful allocation, otherwise error code. */
int allocate_redirection_memory(char ***out_redir, char ***in_redir, int num_commands)
{
	if((*out_redir = calloc(num_commands, sizeof(char*))) == NULL)
		return allocation_error("bc_shell.c: allocate_redirection_memory(): alloc error for out_redir");
	if((*in_redir = calloc(num_commands, sizeof(char*))) == NULL)
		return allocation_error("bc_shell.c: allocate_redirection_memory(): alloc error for in_redir");

	return EXIT_SUCCESS;
}

/* Dynamically allocates memory for an array of pointers to arrays containing two 
   pointers to ints. This is used for allocating memory for pipe file descriptors.
   The caller is responsible for freeing the dynamically allocate memory for
   pc_fd(each pc_fd[i] as well as pc_fd itself).

   Input:  Address of a double pointer to int - Initially undefined, after 
           the function it will hold the arrays containing the pipe file descriptors.

           Integer - the number of pointers to arrays to allocate space for.

   Output: Integer - EXIT_SUCCESS on successful allocation, otherwise error code. */
int allocate_pipe_memory(int ***pc_fd, int num_commands)
{
	int i;
	if((*pc_fd = calloc(num_commands, sizeof(int*))) == NULL)
		return allocation_error("bc_shell.c: allocate_pipe_memory(): alloc error for pc_fd");

	for(i = 0; i < num_commands; i++)
	{
		if(((*pc_fd)[i] = calloc(2, sizeof(int))) == NULL)
			return allocation_error("bc_shell.c: allocate_pipe_memory(): alloc error for pc_fd[i]");
	}

	return EXIT_SUCCESS;
}

/* Checks a string for an absolute or relative location prefix.

   Input:  C string - string containing filename.

   Output: Integer - 1 if the filename has a absolute or relative 
           prefix, otherwise 0. */
int abs_rel_check(char *s)
{
	return prefixcmp_igncase(s, "/")  ||
	       prefixcmp_igncase(s, "./") ||
	       prefixcmp_igncase(s, "../");
}

/* Reports an allocation error.

   Input:  C string - name of the allocation attempted.

   Output: Integer - error code for allocation error. */
int allocation_error(char *name)
{
	perror(name);
	fprintf(stderr, "Press enter to exit.\n");
	getchar();

	return ALLOC_ERROR;
}

/* Reports a path parsing error.

   Input:  None.

   Output: Integer - error code for allocation error. */
int parse_env_var_error()
{
	fprintf(stderr, "Error parsing environment variables\n");
	fprintf(stderr, "Press enter to exit.\n");
	getchar();

	return PARSE_ENV_VAR_ERROR;
}

