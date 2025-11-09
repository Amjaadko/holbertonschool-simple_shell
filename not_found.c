#include "simple_01.h"

#define HSH_NAME "./hsh"

/**
 * utoa_dec - Convert unsigned long to decimal string in @buf
 * @v: value
 * @buf: output buffer (>= 32 bytes)
 * Return: pointer to start of the number inside @buf
 */
static char *utoa_dec(unsigned long v, char *buf)
{
	int i = 31;

	buf[i--] = '\0';
	if (v == 0)
		buf[i--] = '0';
	while (v > 0 && i >= 0)
	{
		buf[i--] = (char)('0' + (v % 10));
		v /= 10;
	}
	return (&buf[i + 1]);
}

/**
 * write_not_found - Print: "./hsh: N: cmd: not found\n"
 * @count: command index starting at 1
 * @cmd: command token
 */
void write_not_found(unsigned long count, const char *cmd)
{
	char num[32];
	char *pnum = utoa_dec(count, num);

	write(STDERR_FILENO, HSH_NAME, sizeof(HSH_NAME) - 1);
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, pnum, strlen(pnum));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, cmd, strlen(cmd));
	write(STDERR_FILENO, ": not found\n", 12);
}

