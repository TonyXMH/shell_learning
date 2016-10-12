/*************************************************************************
	> File Name: uninterruptible_status.c
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月12日 星期三 18时33分35秒
 ************************************************************************/

#include<stdio.h>
int main()
{
	if(!vfork())
		sleep(10);
	return 0;
}
