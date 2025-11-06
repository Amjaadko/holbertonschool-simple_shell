#include "shell.h"

/**
 * env_get - Retrieve the value of an environment variable
 * @name: Variable name (e.g., "PATH")
 *
 * Return: Pointer to value in environ, or NULL if not found
 */
char *env_get(const char *name)
{
	size_t n;
	int i;

	if (!name)
		return (NULL);

	n = strlen(name);
	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], name, n) == 0 && environ[i][n] == '=')
			return (environ[i] + n + 1);
	}
	return (NULL);
}

