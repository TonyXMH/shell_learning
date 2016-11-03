/*************************************************************************
	> File Name: my_unp.h
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年11月03日 星期四 11时27分32秒
 ************************************************************************/

#ifndef __MY_UNP_H
#define __MY_UNP_H

void err_dump(const char *, ...);			//这5个函数都是调用err_doit()函数，只是其中的某些设置不同
void err_msg(const char *, ...);			//err_doit()是静态的所以不在头文件中。
void err_quit(const char *, ..,);
void err_ret(const char *, ...);
void err_sys(const char *, ...);
//
#endif					//__MY_UNP_H
