#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

extern char **environ;

/* string / parsing */
char *trim_whitespace(char *s);
int parse_input(char *cmd, char **argv);

/* PATH + exec */
char *find_path(const char *cmd);
int execute_child(char **argv);

/* env helper */
char *env_get(const char *name);

/* not-found formatter */
void write_not_found(unsigned long count, const char *cmd);

#endif /* SHELL_H */

