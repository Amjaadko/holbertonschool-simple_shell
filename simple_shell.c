#include "shell.h"

/**
 * main - simple shell supporting PATH and relative/absolute paths
 *
 * Read lines with getline, tokenize, search PATH via find_command,
 * do not fork if command not found, print proper error messages and
 * return last status on exit.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *argv[MAX_ARGS];
	char *token;
	int i;
	pid_t pid;
	int status = 0;
	char *cmd_path;

	while (1)
	{
		/* print prompt only in interactive mode */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, ":) ", 3);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			/* EOF (Ctrl+D) or read error */
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* strip newline */
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* skip empty lines */
		if (line[0] == '\0')
			continue;

		/* tokenize into argv[] */
		i = 0;
		token = strtok(line, " \t");
		while (token != NULL && i < (MAX_ARGS - 1))
		{
			argv[i++] = token;
			token = strtok(NULL, " \t");
		}
		argv[i] = NULL;

		if (argv[0] == NULL)
			continue;

		/* built-in exit */
		if (strcmp(argv[0], "exit") == 0)
			break;

		/* If command contains a slash -> treat as path (absolute or relative) */
		if (strchr(argv[0], '/') != NULL)
		{
			if (access(argv[0], X_OK) != 0)
			{
				/* command file not found or not executable */
				dprintf(STDERR_FILENO, "./hsh: 1: %s: not found\n", argv[0]);
				status = 127;
				continue; /* do not fork */
			}
			cmd_path = argv[0];
		}
		else
		{
			/* search in PATH */
			cmd_path = find_command(argv[0]);
			if (cmd_path == NULL)
			{
				dprintf(STDERR_FILENO, "./hsh: 1: %s: not found\n", argv[0]);
				status = 127;
				continue; /* do not fork */
			}
		}

		/* fork and execute */
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(line);
			return (1);
		}
		if (pid == 0)
		{
			execve(cmd_path, argv, environ);
			/* if execve returns, it's an error */
			dprintf(STDERR_FILENO, "./hsh: 1: %s: not found\n", argv[0]);
			_exit(127);
		}
		else
		{
			waitpid(pid, &status, 0);
		}
	}

	free(line);
	return (status);
}

