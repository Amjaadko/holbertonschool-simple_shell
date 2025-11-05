#include "shell.h"

/**
 * main - Simple shell program
 * @ac: argument count
 * @av: argument vector
 * @envp: environment variables
 * Return: 0 on success
 */
int main(int ac, char **av, char **envp)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *args[64];
    int i;
    pid_t pid;
    (void)ac;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(1, ":) ", 3);

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            free(line);
            exit(0);
        }

        /* إزالة \n */
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        /* تجاهل الأسطر الفارغة */
        if (line[0] == '\0')
            continue;

        /* تقسيم الإدخال إلى كلمات */
        i = 0;
        args[i] = strtok(line, " ");
        while (args[i] != NULL && i < 63)
        {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL;

        /* أمر exit */
        if (strcmp(args[0], "exit") == 0)
        {
            free(line);
            exit(0);
        }

        pid = fork();
        if (pid == -1)
        {
            perror("Error");
            free(line);
            exit(1);
        }

        if (pid == 0)
        {
            /* إذا فيه / في الاسم، نفذ مباشرة */
            if (strchr(args[0], '/'))
            {
                if (access(args[0], X_OK) == 0)
                {
                    execve(args[0], args, envp);
                    perror("./hsh");
                    exit(127);
                }
                else
                {
                    perror("./hsh");
                    exit(127);
                }
            }
            else
            {
                /* البحث داخل PATH */
                char *path = getenv("PATH");
                char *path_copy, *dir, full_path[1024];

                if (path == NULL || *path == '\0')
                {
                    perror("./hsh");
                    exit(127);
                }

                path_copy = strdup(path);
                dir = strtok(path_copy, ":");

                while (dir != NULL)
                {
                    snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[0]);
                    if (access(full_path, X_OK) == 0)
                    {
                        execve(full_path, args, envp);
                        perror("./hsh");
                        free(path_copy);
                        exit(127);
                    }
                    dir = strtok(NULL, ":");
                }

                free(path_copy);
                perror("./hsh");
                exit(127);
            }
        }
        else
        {
            wait(NULL);
        }
    }

    free(line);
    return (0

