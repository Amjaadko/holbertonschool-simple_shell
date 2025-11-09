#include "simple_01.h"

/**
 * is_exec - Check if a path exists and is executable
 * @p: Path to check
 * Return: 1 if regular file and executable, 0 otherwise
 */
static int is_exec(const char *p)
{
	struct stat st;

	return (p && stat(p, &st) == 0 && S_ISREG(st.st_mode)
		&& access(p, X_OK) == 0);
}

/**
 * next_path_component - Copy one PATH component into dirbuf
 * @start: current scan position in PATH
 * @nextp: out pointer to next position after component (or NULL if end)
 * @dirbuf: output buffer for component ('.' if empty)
 * @sz: size of dirbuf
 * Return: 1 if a component was produced, 0 if PATH ended
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
 * build_full - Build "dir/leaf" into @full
 * @full: output buffer
 * @fsz: size of @full
 * @dir: directory component
 * @leaf: filename
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
 * find_path - Resolve command using PATH (empty entries mean ".")
 * @cmd: Command name (e.g., "ls") or path (e.g., "/bin/ls")
 * Return: Pointer to static buffer with full path, or NULL if not found
 */
char *find_path(char *cmd)
{
	static char full[1024], dir[512];
	const char *scan, *next;

	if (!cmd || *cmd == '\0')
		return (NULL);

	if (strchr(cmd, '/'))
		return (is_exec(cmd) ? cmd : NULL);

	scan = env_get("PATH");
	if (!scan || *scan == '\0')
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

