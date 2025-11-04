#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

extern char **environ;

/* دالة تبحث عن الأمر داخل PATH */
char *find_path(char *command)
{
    char *path = getenv("PATH");
    char *path_copy;
    char *dir;
    static char full_path[1024];

    if (!path)
        return NULL;

    path_copy = strdup(path);
    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

int main(void)
{
    char input[MAX_INPUT];
    char *argv[MAX_ARGS];
    pid_t pid;
    int status;
    int i;
    char *token;
    char *cmd_path;

    while (1)
    {
        printf("$ ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break; /* Ctrl+D */
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        /* تقسيم النص إلى أوامر */
        i = 0;
        token = strtok(input, " ");
        while (token != NULL && i < MAX_ARGS - 1)
        {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        if (strcmp(argv[0], "exit") == 0)
            break;

        /* تحققي من المسار */
        cmd_path = argv[0];
        if (access(cmd_path, X_OK) != 0)
        {
            cmd_path = find_path(argv[0]);
            if (cmd_path == NULL)
            {
                fprintf(stderr, "%s: command not found\n", argv[0]);
                continue; /* ما يسوي fork */
            }
        }

        pid = fork();
        if (pid == 0)
        {
            execve(cmd_path, argv, environ);
            perror("execve");
            exit(1);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
        }
        else
        {
            perror("fork");
        }
    }

    return 0;
}

