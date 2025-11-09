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

    if (pid == 0) /* Child process */
    {
        execve(argv[0], argv, environ);
        /* If execve fails */
        _exit(errno == EACCES ? 126 : 127);
    }
    else /* Parent process */
    {
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("./hsh");
            return (1);
        }
    }

    /* Return actual exit status of child process */
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        return 128 + WTERMSIG(status); /* terminated by signal */
    else
        return 1;
}

