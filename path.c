#include "shell.h"

/**
 * find_command - تبحث عن الأمر داخل PATH
 * @command: اسم الأمر
 * Return: المسار الكامل إذا وُجد، أو NULL
 */
char *find_command(char *command)
{
    char *path, *path_copy, *dir;
    char *full_path = NULL;
    size_t len;
    struct stat st;
    int i, j;

    /* إذا كان الأمر يبدأ بـ / نستخدمه مباشرة */
    if (command[0] == '/')
    {
        if (stat(command, &st) == 0)
            return (strdup(command));
        return (NULL);
    }

    /* نحصل على PATH من البيئة بدون getenv */
    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path = environ[i] + 5;
            break;
        }
    }
    if (!environ[i] || !path || path[0] == '\0')
        return (NULL);

    path_copy = strdup(path);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        len = strlen(dir) + strlen(command) + 2;
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_copy);
            return (NULL);
        }
        for (j = 0; dir[j]; j++)
            full_path[j] = dir[j];
        full_path[j] = '/';
        strcpy(full_path + j + 1, command);

        if (stat(full_path, &st) == 0)
        {
            free(path_copy);
            return (full_path);
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}

