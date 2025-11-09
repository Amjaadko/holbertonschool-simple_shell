#include "simple_01.h"

/**
 * run_command_line - Resolve via PATH then exec or print not-found
 * @argv: argv tokens (argv[0] is command)
 * @count: command index (for error format)
 * @last_status: out param for status
 * Return: 1 if executed, 0 if not found
 */
int run_command_line(char **argv, unsigned long count, int *last_status)
{
    char *resolved;

    resolved = find_path(argv[0]);
    if (!resolved)
    {
        write_not_found(count, argv[0]);
        *last_status = 127;
        return (0);
    }

    argv[0] = resolved;
    /* Set last_status to whatever the child returns */
    *last_status = execute_child(argv);
    return (1);
}

