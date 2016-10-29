/*************************************************************************
	> File Name: gettimeofday.c
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月29日 星期六 21时21分22秒
 ************************************************************************/

#include<stdio.h>
#include<sys/time.h>

int main()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	printf("tv.tv_sec:%d\n", tv.tv_sec);		//从Epoch开始到当前的秒数
	printf("tv.tv_usec:%d\n", tv.tv_usec);
	return 0;

}
