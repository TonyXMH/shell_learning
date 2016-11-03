/*************************************************************************
	> File Name: error.c
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年11月03日 星期四 11时37分29秒
 ************************************************************************/
#include "my_unp.h"
#include <stdarg.h>
#include <syslog.h>

int daemon_proc;		//作用不明

static void err_doit(int, int, const char *, va_list);

//关于系统调用  非致命的错误，打印错误信息并返回
void
err_ret(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, LOG_INFO, fmt, ap);
	va_end(ap, fmt);
	return;
}
//关于系统调用   致命错误，打印错误信息并终止程序
void
err_sys(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap, fmt);
	exit(1);
}
//关于系统调用   致命错误，打印错误信息 核心以转储 并终止程序
void
err_dump(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, list);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap, list);
	abort();
	exit(1);
}
//与系统调用无关  致命错误，打印错误信息 并终止程序
void
err_quit(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, list);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap, list);
	exit(1);
}

static void
err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
	int errno_save, n;
	char buf[MAXLINE + 1];

	errno_save = errno;
#ifdef	HAVE_VSNPRINTF
	vsnprintf(buf, MAXLINE, fmt, ap);
#else
	vsprintf(buf, fmt, ap);
#endif
	n = strlen(buf);
	if(errnoflag)
		snprintf(buf + n, MAXLINE - n, ":%s", strerrno(errno_save));
	strcat(buf, "\n");

	if(daemon_proc)
		syslog(level, buf);
	else
	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}
	return;
}
