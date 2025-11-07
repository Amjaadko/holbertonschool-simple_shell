#include "shell.h"

static int is_exec(const char *p)
{
    struct stat st;
    return (p && stat(p, &st) == 0 && S_ISREG(st.st_mode)
            && access(p, X_OK) == 0);
}

char *find_path(const char *cmd)
{
    static char full[1024];
    char *path_env, *path_dup, *dir;
    char *saveptr;

    if (!cmd || strchr(cmd, '/'))
        return is_exec(cmd) ? (char *)cmd : NULL;

    path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    path_dup = strdup(path_env);
    if (!path_dup)
        return NULL;

    dir = strtok_r(path_dup, ":", &saveptr);
    while (dir)
    {
        snprintf(full, sizeof(full), "%s/%s", dir, cmd);
        if (is_exec(full))
        {
            free(path_dup);
            return full;
        }
        dir = strtok_r(NULL, ":", &saveptr);
    }

    free(path_dup);
    return NULL;
}

