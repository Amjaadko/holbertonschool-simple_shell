#include "shell.h"
#include <sys/stat.h>

/* Check if file exists and executable */
static int is_exec(const char *path)
{
    struct stat st;
    return path && stat(path, &st) == 0 && S_ISREG(st.st_mode) && access(path, X_OK) == 0;
}

/* Build full path dir + / + cmd */
static int build_full(char *full, size_t fsz, const char *dir, const char *cmd)
{
    size_t dl = strlen(dir), cl = strlen(cmd);
    if (dl + 1 + cl + 1 > fsz)
        return 0;
    memcpy(full, dir, dl);
    full[dl] = '/';
    memcpy(full + dl + 1, cmd, cl);
    full[dl + 1 + cl] = '\0';
    return 1;
}

/* Return full path to command using PATH */
char *find_path(const char *cmd)
{
    static char full[1024];
    char *path_copy, *dir;
    const char *env_path;

    if (!cmd || *cmd == '\0')
        return NULL;

    /* If command contains '/', check it directly */
    if (strchr(cmd, '/'))
        return is_exec(cmd) ? (char *)cmd : NULL;

    env_path = env_get("PATH");
    if (!env_path)
        return NULL;

    path_copy = strdup(env_path);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        if (build_full(full, sizeof(full), dir, cmd) && is_exec(full))
        {
            free(path_copy);
            return full;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

