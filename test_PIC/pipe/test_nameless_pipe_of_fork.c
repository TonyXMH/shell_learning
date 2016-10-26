/*************************************************************************
	> File Name: test_nameless_pipe_of_fork.c
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月26日 星期三 11时15分47秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<memory.h>
#include<stdlib.h>
#include<sys/wait.h>
#define PIPE_SIZE 2
#define BUF_SIZE 100
#define WRITE_SIZE 5
int main(void)
{
	pid_t pid;
	int pipe_fd[PIPE_SIZE];
	int read_num;
	char buf[BUF_SIZE];

	memset(buf, 0, BUF_SIZE);
	if(pipe(pipe_fd) < 0)
	{
		perror("pipe\n");
		return -1;
	}
	if(0 == (pid = fork()))		//子进程中读
	{
		close(pipe_fd[1]);		//关闭写
		sleep(5);
		if((read_num = read(pipe_fd[0], buf, BUF_SIZE)) < 0)
		{
			perror("child process read\n");
			close(pipe_fd[0]);
			exit(-1);
		}
		fprintf(stdout, "child process read succeed\n the contain is %s", buf);
		close(pipe_fd[0]);
		exit(0);
	}
	else						//父进程中写
	{
		close(pipe_fd[0]);		//关闭读
		if((write(pipe_fd[1], "HELL", WRITE_SIZE)) == WRITE_SIZE)
		{
			fprintf(stdout, "parent process write1 succeed\n");
		}
		else
		{
			perror("parent process write1 failed\n");
			close(pipe_fd[1]);
			exit(-1);
		}
		if((write(pipe_fd[1], "word", WRITE_SIZE)) == WRITE_SIZE)
		{
			fprintf(stdout, "parent process write2 succeed\n");
		}
		else
		{
			perror("parent process write2 failed\n");
			close(pipe_fd[1]);
			exit(-1);
		}
		close(pipe_fd[1]);
		sleep(3);
		waitpid(pid, NULL, 0);
		
	}

	return 0;
}
