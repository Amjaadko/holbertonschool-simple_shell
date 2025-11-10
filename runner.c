#include "simple_01.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int run_command_line(char **argv, unsigned long count, int *last_status)
{
    int status;

    (void)count;

    if (argv == NULL || argv[0] == NULL)
        return (1);

    /* أمر exit */
    if (strcmp(argv[0], "exit") == 0)
    {
        int code = (*last_status == 0) ? 0 : *last_status;
        exit(code);
    }

    /* تنفيذ الأمر */
    status = execute_child(argv);

    /* نخزن آخر status */
    *last_status = status;

    return (status);
}

