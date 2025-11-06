#include "shell.h"

/**
 * write_not_found - Prints an error message when a command is not found
 * @count: The number of the command (for formatting)
 * @cmd: The command name that was not found
 */
void write_not_found(unsigned long count, const char *cmd)
{
    char buffer[128];
    int len;

    len = snprintf(buffer, sizeof(buffer), "./hsh: %lu: %s: not found\n", count, cmd);
    write(STDERR_FILENO, buffer, len);
}

/**
 * execute_child - Fork and execve using argv[0] as resolved path
 * @argv: Argument vector; argv[0] must be an executable path
 *
 * Return: Child exit status (0..255), 126 on EACCES, 127 on ENOENT
 */
int execute_child(char **argv)
{
	pid_t pid;
	int status = 0;

	pid = fork();
	if (pid < 0)
	{
		perror("./hsh");
		return (1);
	}

	if (pid == 0)
	{
		execve(argv[0], argv, environ);
		perror("./hsh");
		_exit(errno == EACCES ? 126 : 127);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			perror("./hsh");
	}

	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

