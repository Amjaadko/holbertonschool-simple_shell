#include "shell.h"

/**
 * main - simple shell 0.3
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *argv[64];
    int i, status;
    pid_t pid;

    while (1)
    {
        write(STDOUT_FILENO, ":) ", 3);
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        argv[0] = strtok(line, " ");
        if (argv[0] == NULL)
            continue;

        for (i = 1; i < 63; i++)
        {
            argv[i] = strtok(NULL, " ");
            if (argv[i] == NULL)
                break;
        }
        argv[i] = NULL;

        /* ✅ إذا فيه '/'، نفذ مباشرة من المسار */
        if (strchr(argv[0], '/'))
        {
            if (access(argv[0], X_OK) != 0)
            {
                dprintf(STDERR_FILENO, "./hsh: 1: %s: not found\n", argv[0]);
                continue;
            }
        }
        else
        {
            char *cmd_path = find_command(argv[0]);
            if (!cmd_path)
            {
                dprintf(STDERR_FILENO, "./hsh: 1: %s: not found\n", argv[0]);
                continue;
            }
            argv[0] = cmd_path;
        }

        pid = fork();
        if (pid == 0)
        {
            execve(argv[0], argv, environ);
            dprintf(STDERR_FILENO, "./hsh: 1: %s: not found\n", argv[0]);
            exit(127);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return (0);
}

