#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
extern char **environ;
/**
 * find_path - تبحث عن الأمر داخل PATH وتعيد المسار الكامل له
 */
char *find_path(char *command)
{
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    static char full_path[1024];

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

    while (1)
    {
        printf("$ ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0')
            continue;

        /* تقسيم السطر إلى أوامر */
        argv[0] = strtok(input, " ");
        int i = 0;
        while (argv[i] != NULL && i < MAX_ARGS - 1)
            argv[++i] = strtok(NULL, " ");
        argv[i] = NULL;

        /* الأمر exit */
        if (strcmp(argv[0], "exit") == 0)
            break;

        /* التحقق من وجود المسار */
        char *cmd_path = argv[0];
        if (access(cmd_path, X_OK) != 0)
            cmd_path = find_path(argv[0]);

        if (cmd_path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            execve(cmd_path, argv, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
        {
            perror("fork");
        }
        else
        {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

