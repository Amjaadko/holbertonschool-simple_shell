#include "shell.h"
#include <unistd.h>
#include <stdio.h>

void write_not_found(unsigned long count, const char *cmd)
{
    char buffer[128];
    int len;

    len = snprintf(buffer, sizeof(buffer), "./hsh: %lu: %s: not found\n", count, cmd);
    write(STDERR_FILENO, buffer, len);
}

