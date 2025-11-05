#include "shell.h"

/**
 * find_command_in_path - يبحث عن المسار الكامل للأمر داخل PATH
 * @command: اسم الأمر
 * Return: المسار الكامل إذا وُجد، NULL إذا لم يوجد
 */
char *find_command_in_path(char *command)
{
    char *path, *path_copy, *token, *full_path;
    size_t len;
    int access_result;

    if (access(command, X_OK) == 0)
        return strdup(command);

    path = getenv("PATH");
    if (!path)
        return NULL;

    path_copy = strdup(path);
    token = strtok(path_copy, ":");

    while (token != NULL)
    {
        len = strlen(token) + strlen(command) + 2;
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }

        snprintf(full_path, len, "%s/%s", token, command);
        access_result = access(full_path, X_OK);

        if (access_result == 0)
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

