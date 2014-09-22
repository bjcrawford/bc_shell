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

/* Function declarations */
int shellPrompt();
int parseInput(int*, char***);
int allocation_error(char*);
int chopTest();

int main(int argc, char **argv)
{
	return shellPrompt();
}

int shellPrompt()
{
	int i;
	int i_argc;
	char **i_argv;
	char **dp;

	while(parseInput(&i_argc, &i_argv))
	{
		/*
		printf("i_argc: %d\n", i_argc);
		dp = i_argv;
		while(*dp != NULL)
		{
			printf("argv[%d]: %s\n", dp - i_argv, *dp);
			dp++;
		}
		*/

		if(access(i_argv[0], F_OK) != -1)
		{
			// Command exists
			printf("Running command %s\n", i_argv[0]);
		}
		else
		{
			// Command not found
			printf("-bc_shell: %s: command not found\n", i_argv[0]);
		}
		
		for(i = 0; i_argv[i] != NULL; i++)
			free(i_argv[i]);
		free(i_argv);
	}

	

	printf("Exiting...\n");

	return EXIT_SUCCESS;
}

int parseInput(int *i_argc, char ***i_argv)
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

int chopTest()
{
	printf("\n\n====== chop() tests start ======\n\n");
	char s47[] = "I am ready for a nice vacation.";
	char **words = chop(s47, ' ');
	int i = 0;
	printf("%d: %s\n", i, words[i]);
	do {
		i++;
		printf("%d: %s\n", i, words[i]);
	} while(words[i] != NULL);
	free(words);

	return EXIT_SUCCESS;
}
