#include "shell.h"

/**
 * find_command - search for command in PATH or accept direct paths
 * @command: command name (may be "ls" or "./prog" or "../prog" or "/bin/prog")
 *
 * Return: pointer to a string that can be passed to execve:
 *  - If command is absolute or relative and executable: return command
 *  - If found in PATH: return pointer to static buffer containing full path
 *  - If not found: return NULL
 *
 * Notes:
 *  - returns pointer to static buffer for PATH-found results.
 *  - caller must not free returned pointer when it points to static buffer.
 */
char *find_command(char *command)
{
	char *path;
	char *path_copy;
	char *dir;
	static char full_path[1024];
	int i;

	/* If command contains a slash (absolute or relative path), check directly */
	if (command[0] == '/' || strncmp(command, "./", 2) == 0 ||
	    strncmp(command, "../", 3) == 0)
	{
		if (access(command, X_OK) == 0)
			return (command);
		return (NULL);
	}

	/* find PATH in environ (no getenv) */
	path = NULL;
	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			break;
		}
	}
	if (path == NULL || path[0] == '\0')
		return (NULL);

	path_copy = strdup(path);
	if (path_copy == NULL)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

