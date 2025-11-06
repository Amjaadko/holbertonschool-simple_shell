#include "shell.h"

/**
 * main - Entry point for the simple shell
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL, *cmd_path = NULL;
    size_t len = 0;
    ssize_t read;
    char *argv[64];
    int status = 0;
    unsigned long cmd_count = 0;

    while (1)
    {
        cmd_count++;
        write(STDOUT_FILENO, ":) ", 3);

        read = getline(&line, &len, stdin);
        if (read == -1)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* إزالة نهاية السطر */
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        if (line[0] == '\0')
            continue;

        /* تقسيم الإدخال إلى كلمات */
        int i = 0;
        char *token = strtok(line, " ");
        while (token && i < 63)
        {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        if (strcmp(argv[0], "exit") == 0)
            break;

        /* البحث عن الأمر في PATH */
        cmd_path = find_path(argv[0]);
        if (!cmd_path)
        {
            write_not_found(cmd_count, argv[0]);
            continue;
        }

        /* تنفيذ الأمر */
        argv[0] = cmd_path;
        status = execute_child(argv);
    }

    free(line);
    return (status);
}

