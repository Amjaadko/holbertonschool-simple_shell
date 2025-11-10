#include "simple_01.h"

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

