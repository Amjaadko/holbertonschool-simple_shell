#include "shell.h"

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    char *argv[MAX_ARGS];
    pid_t pid;
    int status;
    char *token;
    int i;
    char *cmd_path;

    while (1)
    {
        /* عرض prompt في الوضع التفاعلي فقط */
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, ":) ", 3);

        /* قراءة السطر من المستخدم */
        if (getline(&line, &len, stdin) == -1)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* إزالة السطر الجديد */
        line[strcspn(line, "\n")] = '\0';

        /* تقسيم الأمر إلى argv */
        i = 0;
        token = strtok(line, " ");
        while (token != NULL && i < MAX_ARGS - 1)
        {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        /* لو المستخدم ضغط Enter فقط */
        if (argv[0] == NULL)
            continue;

        /* إيجاد المسار الكامل للأمر */
        cmd_path = find_command(argv[0]);
        if (cmd_path == NULL)
        {
            write(STDERR_FILENO, argv[0], strlen(argv[0]));
            write(STDERR_FILENO, ": command not found\n", 20);
            continue;
        }

        /* تنفيذ الأمر */
        pid = fork();
        if (pid == 0)
        {
            execve(cmd_path, argv, environ);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
        }
        else
        {
            perror("fork");
        }
    }

    free(line);
    return 0;
}

