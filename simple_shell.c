#include "shell.h"

/**
 * main - Simple shell 0.3
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL, *token = NULL, *cmd_path = NULL;
    size_t len = 0;
    ssize_t read;
    char *argv[64];
    int status = 0, i;
    pid_t pid;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, ":) ", 3);

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            free(line);
            exit(status);
        }

        /* إزالة السطر الجديد */
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        if (line[0] == '\0')
            continue;

        token = strtok(line, " ");
        i = 0;
        while (token != NULL && i < 63)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        /* البحث عن المسار */
        cmd_path = find_command(argv[0]);
        if (cmd_path == NULL)
        {
            dprintf(STDERR_FILENO, "./hsh: 1: %s: not found\n", argv[0]);
            status = 127;
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(line);
            free(cmd_path);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            if (execve(cmd_path, argv, environ) == -1)
            {
                perror("./hsh");
                free(line);
                free(cmd_path);
                exit(2);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
            free(cmd_path);
        }
    }

    free(line);
    return (status);
}

