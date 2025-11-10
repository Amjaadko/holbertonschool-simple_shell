#include "simple_01.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

extern char **environ;

int execute_child(char **argv)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }

    if (pid == 0)
    {
        /* child process */
        if (execve(argv[0], argv, environ) == -1)
        {
            perror(argv[0]);
            _exit(2); /* مهم جدًا */
        }
    }
    else
    {
        /* parent process */
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        else
            return (2);
    }

    return (2);
}

