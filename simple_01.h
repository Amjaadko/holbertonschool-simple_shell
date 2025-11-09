#ifndef SIMPLE_01_H
#define SIMPLE_01_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PROMPT "$ "
#define MAX_ARGS 64

extern char **environ;

/* string / parsing */
char *trim_whitespace(char *s);
int parse_input(char *cmd, char **argv_exec);
int shell_loop(void);

/* PATH + exec */
char *find_path(char *cmd);
int execute_child(char **argv);

/* env helper (no getenv) */
char *env_get(const char *name);

/* not-found formatter */
void write_not_found(unsigned long count, const char *cmd);

/* builtins + runner */
int try_builtin(char **argv, int *last_status, int *should_exit);
int run_command_line(char **argv, unsigned long count, int *last_status);

#endif /* SIMPLE_01_H */

