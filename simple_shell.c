#include "shell.h"

int main(void)
{
    char *line = NULL, *argv[64];
    ssize_t nread;
    size_t len = 0;
    pid_t pid;
    int status;

    while (1)
    {
        printf(":) ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            printf("\n");
            break;
        }

        /* إزالة الـ newline */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* تقسيم السطر إلى أوامر */
        char *token = strtok(line, " ");
        int i = 0;
        while (token && i < 63)
        {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        if (argv[0] == NULL)
            continue;

        /* إذا كتب exit */
        if (strcmp(argv[0], "exit") == 0)
            break;

        /* البحث عن الأمر في PATH */
        char *cmd_path = find_command(argv[0]);
        if (cmd_path == NULL)
        {
            fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
            continue; /* لا يتم تنفيذ fork */
        }

        /* تنفيذ الأمر */
        pid = fork();
        if (pid == 0)
        {
            execve(cmd_path, argv, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            wait(&status);
            free(cmd_path);
        }
        else
        {
            perror("fork");
        }
    }

    free(line);
    return 0;
}

