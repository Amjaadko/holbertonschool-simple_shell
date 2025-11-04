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
 * @command: اسم الأمر المراد تنفيذه
 * Return: المؤشر إلى المسار الكامل أو NULL إذا لم يتم العثور عليه
 */
char *find_path(char *command)
{
    char *path, *path_copy, *dir;
    static char full_path[1024];

    path = getenv("PATH");
    if (path == NULL)
        return (NULL);

    path_copy = strdup(path);
    if (path_copy == NULL)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (full_path);
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return (NULL);
}

/**
 * main - برنامج shell بسيط
 * Return: 0 عند الخروج
 */
int main(void)
{
    char input[MAX_INPUT];
    char *argv[MAX_ARGS];
    pid_t pid;
    int status;
    int i;
    char *token;
    char *cmd_path;
    char *delim = " \t\r\n";

    while (1)
    {
        printf("$ ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        /* إزالة نهاية السطر */
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        /* تقسيم الإدخال إلى كلمات */
        token = strtok(input, delim);
        i = 0;
        while (token != NULL && i < MAX_ARGS - 1)
        {
            argv[i++] = token;
            token = strtok(NULL, delim);
        }
        argv[i] = NULL;

        if (argv[0] == NULL)
            continue;

        /* أمر الخروج */
        if (strcmp(argv[0], "exit") == 0)
            break;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        else if (pid == 0)
        {
            /* في عملية الابن */
            if (strchr(argv[0], '/') != NULL)
                cmd_path = argv[0];
            else
                cmd_path = find_path(argv[0]);

            if (cmd_path == NULL)
            {
                fprintf(stderr, "%s: command not found\n", argv[0]);
                exit(127);
            }

            execve(cmd_path, argv, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* في عملية الأب */
            waitpid(pid, &status, 0);
        }
    }

    return (0);
}

