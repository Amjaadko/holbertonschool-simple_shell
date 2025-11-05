#include "shell.h"

char *find_command(char *command)
{
    char *path = NULL, *dir, *path_copy;
    static char full_path[1024];
    int i = 0;

    /* إذا كان الأمر يبدأ بـ / أو . نرجعه مباشرة */
    if (command[0] == '/' || command[0] == '.')
        return command;

    /* نحصل على PATH من environ يدويًا */
    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path = environ[i] + 5;
            break;
        }
        i++;
    }

    if (!path)
        return NULL;

    path_copy = strdup(path);
    if (!path_copy)
        return NULL;

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

