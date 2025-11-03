/* simple_shell.c - Simple shell 1.1 (fixed)
 * Compile:
 * gcc -Wall -Werror -Wextra -pedantic -std=gnu89 simple_shell.c -o simple_shell
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

#define PROMPT "$ "

/* helper: trim leading and trailing whitespace in-place, returns pointer to trimmed */
char *trim(char *s)
{
    char *end;

    if (s == NULL)
        return NULL;

    /* trim leading */
    while (*s == ' ' || *s == '\t' || *s == '\n')
        s++;

    if (*s == '\0') /* empty string */
        return s;

    /* trim trailing */
    end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\n'))
    {
        *end = '\0';
        end--;
    }
    return s;
}

int main(void)
{
    char *lineptr = NULL;
    size_t n = 0;
    ssize_t nchars_read;
    pid_t child_pid;
    int status;
    char *argv_exec[2];
    char *cmd;

    while (1)
    {
        /* print prompt only when interactive */
        if (isatty(STDIN_FILENO))
        {
            if (write(STDOUT_FILENO, PROMPT, strlen(PROMPT)) == -1)
            {
                /* not fatal — continue to try reading */
                perror("write");
            }
        }

        nchars_read = getline(&lineptr, &n, stdin);
        if (nchars_read == -1)
        {
            /* EOF (Ctrl+D) or read error */
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* ensure null-terminated and trim whitespace */
        if (nchars_read > 0 && lineptr[nchars_read - 1] == '\n')
            lineptr[nchars_read - 1] = '\0';

        cmd = trim(lineptr);

        /* ignore empty or whitespace-only input */
        if (cmd == NULL || *cmd == '\0')
            continue;

        /* ignore accidental input of the prompt symbol alone */
        if (strcmp(cmd, PROMPT) == 0 || (cmd[0] == '$' && cmd[1] == '\0'))
            continue;

        /* handle built-in exit */
        if (strcmp(cmd, "exit") == 0)
            break;

        /* only first token (no args for task 0.1) */
        /* tokenize by whitespace to ignore trailing garbage */
        cmd = strtok(cmd, " \t");
        if (cmd == NULL || *cmd == '\0')
            continue;

        argv_exec[0] = cmd;
        argv_exec[1] = NULL;

        child_pid = fork();
        if (child_pid == -1)
        {
            perror("fork");
            continue;
        }

        if (child_pid == 0) /* child */
        {
            /* pass environ as required */
            if (execve(argv_exec[0], argv_exec, environ) == -1)
            {
                dprintf(STDERR_FILENO, "%s: %s\n", argv_exec[0], strerror(errno));
                _exit(EXIT_FAILURE);
            }
        }
        else /* parent */
        {
            if (waitpid(child_pid, &status, 0) == -1)
                perror("waitpid");
        }
    }

    free(lineptr);
    return (0);
}

