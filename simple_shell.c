#include "shell.h"

int main(void)
{
    char line[MAX_INPUT];
    char *argv[MAX_ARGS];
    pid_t pid;
    int status;
    char *token;
    int i;
    char *cmd_path;

    while (1)
    {
        /* اطبعي الـ prompt فقط في الوضع التفاعلي */
        if (isatty(STDIN_FILENO))
            printf(":) ");

        /* قراءة الأمر من المستخدم أو من ملف */
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* إزالة السطر الجديد */
        line[strcspn(line, "\n")] = '\0';

        /* تقسيم الإدخال إلى أوامر */
        i = 0;
        token = strtok(line, " ");
        while (token != NULL && i < MAX_ARGS - 1)
        {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        /* إذا المستخدم ضغط Enter فقط */
        if (argv[0] == NULL)
            continue;

        /* إيجاد المسار الكامل للأمر */
        cmd_path = find_command(argv[0]);
        if (cmd_path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            continue; /* ما نسوي fork إذا الأمر غير موجود */
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

    return 0;
}

