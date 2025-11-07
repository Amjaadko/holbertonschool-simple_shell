#include "shell.h"

/* Return pointer to value of environment variable */
char *env_get(const char *name)
{
    int i;
    size_t len;

    if (!name)
        return NULL;

    len = strlen(name);
    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
            return environ[i] + len + 1;
    }
    return NULL;
}

