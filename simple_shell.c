#include "shell.h"

int main(void)
{
    char input[MAX_INPUT];
    char *argv[MAX_ARGS];
    char *token, *cmd_path;
    int i, status;
    unsigned long cmd_count = 0;

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);
        if (!fgets(input, sizeof(input), stdin))
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0')
            continue;

        i = 0;
        token = strtok(input, " ");
        while (token && i < MAX_ARGS - 1)
        {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;
        cmd_count++;

        if (strcmp(argv[0], "exit") == 0)
            break;

        cmd_path = argv[0];
        if (!strchr(cmd_path, '/') && access(cmd_path, X_OK) != 0)
            cmd_path = find_path(argv[0]);

        if (!cmd_path)
        {
            write_not_found(cmd_count, argv[0]);
            continue;
        }

        status = execute_child((char *[]){cmd_path, argv[1], argv[2], argv[3], argv[4], NULL});
        (void)status;
    }

    return 0;
}

