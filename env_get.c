#include "simple_01.h"

/**
 * env_get - Return the value of an environment variable using environ
 * @name: Variable name (e.g., "PATH")
 *
 * Return: Pointer to value within environ or NULL if not found
 */

char *env_get(const char *name)
{
	extern char **environ;
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
