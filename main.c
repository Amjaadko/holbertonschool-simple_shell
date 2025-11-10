#include "simple_01.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char **argv;
    int last_status = 0;
    unsigned long count = 0;

    while (1)
    {
        printf("$ ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;

        argv = parse_line(line);
        if (argv && argv[0])
            run_command_line(argv, count, &last_status);

        free_argv(argv);
        count++;
    }

    free(line);
    return (last_status);
}

