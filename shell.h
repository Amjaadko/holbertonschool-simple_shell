#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

extern char **environ;

/* shell helpers */
char *find_path(const char *command);
char *env_get(const char *name);
int execute_child(char **argv);
void write_not_found(unsigned long count, const char *cmd);

#endif /* SHELL_H */

