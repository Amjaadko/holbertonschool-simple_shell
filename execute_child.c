#include "shell.h"
#include <sys/wait.h>
#include <errno.h>

/* Fork and execve the command, return status */
int execute_child(char **argv)
{
    pid_t pid;
    int status = 0;

    pid = fork();
    if (pid < 0)
    {
        perror("./hsh");
        return 1;
    }

    if (pid == 0)
    {
        execve(argv[0], argv, environ);
        perror("execve");
        _exit(errno == EACCES ? 126 : 127);
    }
    else
    {
        if (waitpid(pid, &status, 0) == -1)
            perror("./hsh");
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

