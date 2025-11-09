#include "simple_01.h"

/**
 * trim_whitespace - Remove leading/trailing spaces/tabs/newlines
 * @s: Input string
 * Return: Pointer to trimmed string (same buffer)
 */
char *trim_whitespace(char *s)
{
	char *end;

	if (!s)
		return (NULL);

	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;

	if (*s == '\0')
		return (s);

	end = s + strlen(s) - 1;
	while (end > s && (*end == ' ' || *end == '\t' || *end == '\n'))
	{
		*end = '\0';
		end--;
	}
	return (s);
}

/**
 * parse_input - Split a command line into tokens
 * @cmd: Input command (modified)
 * @argv_exec: Output array for tokens (MAX_ARGS)
 * Return: Number of tokens
 */
int parse_input(char *cmd, char **argv_exec)
{
	char *tok;
	int i = 0;

	tok = strtok(cmd, " \t");
	while (tok && i < (MAX_ARGS - 1))
	{
		argv_exec[i++] = tok;
		tok = strtok(NULL, " \t");
	}
	argv_exec[i] = NULL;
	return (i);
}

/**
 * shell_loop - prompt → read → parse → builtin/exec
 * Return: last command status
 */
int shell_loop(void)
{
	char *line = NULL, *cmd;
	size_t cap = 0;
	ssize_t n;
	char *argv_exec[MAX_ARGS];
	int last_status = 0, should_exit = 0;
	unsigned long count = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

		n = getline(&line, &cap, stdin);
		if (n == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		if (n > 0 && line[n - 1] == '\n')
			line[n - 1] = '\0';

		cmd = trim_whitespace(line);
		if (!cmd || *cmd == '\0' || parse_input(cmd, argv_exec) == 0)
			continue;

		count++;
		if (try_builtin(argv_exec, &last_status, &should_exit))
		{
			if (should_exit)
				break;
			continue;
		}
		(void)run_command_line(argv_exec, count, &last_status);
	}
	free(line);
	return (last_status);
}

/**
 * main - Entry point
 * Return: exit status from shell_loop
 */
int main(void)
{
	return (shell_loop());
}

