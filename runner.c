#include "simple_01.h"

/* run command line and update last_status */
int run_command_line(char **argv, unsigned long count, int *last_status)
{
    int status;       /* تعريف المتغير في بداية الدالة */
    (void)count;      /* تجاهل count إذا لم تستخدم */

    status = execute_child(argv); /* نفذ الأمر واحصل على حالة الخروج */

    if (last_status)   /* خزّن قيمة الخروج في last_status */
        *last_status = status;

    return status;
}

