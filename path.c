#include "shell.h"

char *find_command(char *command)
{
    char *path = getenv("PATH");
    char *path_copy, *dir;
    static char full_path[1024];

    if (command[0] == '/' || command[0] == '.')
        return command;

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

