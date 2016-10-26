/*************************************************************************
	> File Name: test_nameless_pipe01.c
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月26日 星期三 10时59分47秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
int main(void)
{
	int fd[2];
	if(pipe(fd) < 0)
	{
		perror("pipe\n");
		return 1;
	}
	else
	{
		fprintf(stdout, "pipe succeed\n");
		close(fd[0]);						//关闭读
		close(fd[1]);						//关闭写
	}
	return 0;
}
