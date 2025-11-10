#include "simple_01.h"

/**
 * try_builtin - Handle builtins: exit, env
 * @argv: argv tokens
 * @last_status: out param for status
 * @should_exit: out flag: 1 if shell must exit
 * Return: 1 if a builtin was handled, 0 otherwise
 */
int try_builtin(char **argv, int *last_status, int *should_exit)
{
	int i;

	if (strcmp(argv[0], "exit") == 0)
	{
		*should_exit = 1;
		return (1);
	}

	if (strcmp(argv[0], "env") == 0)
	{
		for (i = 0; environ[i]; i++)
		{
			size_t len = strlen(environ[i]);

			write(STDOUT_FILENO, environ[i], len);
			write(STDOUT_FILENO, "\n", 1);
		}
		*last_status = 0;
		return (1);
	}

	*should_exit = 0;
	return (0);
}

