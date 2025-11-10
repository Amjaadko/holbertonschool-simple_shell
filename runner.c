#include "simple_01.h"

/**
 * run_command_line - Execute a command using PATH resolution
 * @argv: Arguments array (argv[0] is command)
 * @count: Command counter (for error messages)
 * @last_status: Pointer to store child exit status
 * Return: 0 always
 */
int run_command_line(char **argv, unsigned long count, int *last_status)
{
    char *path;

    if (!argv || !argv[0])
        return (0);

    path = find_path(argv[0]);
    if (!path)
    {
        write_not_found(count, argv[0]);
        *last_status = 127;
        return (*last_status);
    }

    *last_status = execute_child(argv);
    return (*last_status);
}

/**
 * main - Entry point of the shell
 * Return: exit status from shell_loop
 */
int main(void)
{
    return shell_loop();
}

