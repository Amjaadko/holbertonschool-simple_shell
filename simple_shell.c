#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *argv_exec[64]; /* array to hold command + arguments */
    pid_t pid;
    int status;

    while (1)
    {
        printf("$ ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            printf("\n");
            break; /* Ctrl+D */
        }

        /* remove newline */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* check for empty input */
        if (line[0] == '\0')
            continue;

        /* check for "exit" */
        if (strcmp(line, "exit") == 0)
            break;

        /* tokenize the input */
        int i = 0;
        char *token = strtok(line, " ");
        while (token != NULL && i < 63)
        {
            argv_exec[i++] = token;
            token = strtok(NULL, " ");
        }
        argv_exec[i] = NULL;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            if (execve(argv_exec[0], argv_exec, NULL) == -1)
                perror(argv_exec[0]);
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}

