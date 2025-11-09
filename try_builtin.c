#include "simple_01.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * try_builtin - handle built-in shell commands
 * @argv: array of command arguments
 * @last_status: pointer to store last command status
 * @should_exit: pointer to flag indicating exit
 * Return: 1 if a built-in command was executed, 0 otherwise
 */
int try_builtin(char **argv, int *last_status, int *should_exit)
{
	*should_exit = 0;

	if (argv[0] == NULL)
		return (0);

	if (strcmp(argv[0], "exit") == 0)
	{
		*should_exit = 1;
		*last_status = 0;
		return (1);
	}

	if (strcmp(argv[0], "env") == 0)
	{
		extern char **environ;
		int i = 0;

		while (environ[i])
		{
			printf("%s\n", environ[i]);
			i++;
		}
		*last_status = 0;
		return (1);
	}

	return (0);
}

