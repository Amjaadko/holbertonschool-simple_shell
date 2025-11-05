#include "shell.h"

/**
 * is_exec - Check if a file exists and is executable
 * @p: path to check
 * Return: 1 if executable regular file, 0 otherwise
 */
static int is_exec(const char *p)
{
    struct stat st;

    return (p && stat(p, &st) == 0 && S_ISREG(st.st_mode)
        && access(p, X_OK) == 0);
}

/**
 * next_path_component - Extract next PATH component
 * @start: current scan position
 * @nextp: pointer to store next position
 * @dirbuf: buffer for directory path
 * @sz: buffer size
 * Return: 1 if got component, 0 if done
 */
static int next_path_component(const char *start,
    const char **nextp, char *dirbuf, size_t sz)
{
    const char *colon;
    size_t len;

    if (!start)
        return (0);

    colon = strchr(start, ':');
    len = colon ? (size_t)(colon - start) : strlen(start);

    if (len == 0)
    {
        dirbuf[0] = '.';
        dirbuf[1] = '\0';
    }
    else
    {
        if (len >= sz)
            len = sz - 1;
        memcpy(dirbuf, start, len);
        dirbuf[len] = '\0';
    }

    *nextp = colon ? (colon + 1) : NULL;
    return (1);
}

/**
 * build_full - Join directory and command into full path
 * @full: buffer for full path
 * @fsz: buffer size
 * @dir: directory
 * @leaf: command
 * Return: 1 on success, 0 on overflow
 */
static int build_full(char *full, size_t fsz,
    const char *dir, const char *leaf)
{
    size_t dl = strlen(dir), ll = strlen(leaf);

    if (dl + 1 + ll + 1 > fsz)
        return (0);

    memcpy(full, dir, dl);
    full[dl] = '/';
    memcpy(full + dl + 1, leaf, ll);
    full[dl + 1 + ll] = '\0';
    return (1);
}

/**
 * find_command - Resolve command path using PATH env
 * @cmd: command name (like "ls")
 * Return: full path (static buffer) or NULL if not found
 */
char *find_command(char *cmd)
{
    static char full[1024], dir[512];
    const char *scan, *next;
    int i = 0;

    if (!cmd || *cmd == '\0')
        return (NULL);

    if (strchr(cmd, '/'))
        return (is_exec(cmd) ? cmd : NULL);

    /* get PATH manually from environ */
    while (environ[i])
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            scan = environ[i] + 5;
            break;
        }
        i++;
    }

    if (!environ[i])
        return (NULL);

    while (next_path_component(scan, &next, dir, sizeof(dir)))
    {
        if (build_full(full, sizeof(full), dir, cmd) && is_exec(full))
            return (full);
        if (!next)
            break;
        scan = next;
    }

    return (NULL);
}

