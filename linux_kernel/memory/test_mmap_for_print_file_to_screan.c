/*************************************************************************
	> File Name: test_mmap_for_print_file_to_screan.c
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月25日 星期二 17时51分54秒
 ************************************************************************/
//用mmap将用户指定的文件 输出到标准输出
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>

int main(int argc, char *argv[])
{
	struct stat sb;
	off_t len;
	char *p;
	int fd;
	if(argc < 2)
	{
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}
	fd = open(argv[1], O_RDONLY);		//只读打开文件
	if(-1 == fd)
	{
		perror("open\n");
		return 1;
	}
	if(-1 == fstat(fd, &sb))			//获取指定文件信息
	{
		perror("fstat\n");
		return 1;
	}
	if(!S_ISREG(sb.st_mode))			//判断文件是否是普通文件（regular）
	{
		fprintf(stderr, "%s is not a file\n", argv[0]);
		return 1;
	}
	p = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);	//创建映射
	if(MAP_FAILED == p)
	{
		perror("mmap\n");
		return 1;
	}
	if(-1 == close(fd))					//关闭文件
	{
		perror("close");
		return 1;
	}
	for(len = 0; len < sb.st_size; ++len)	
		putchar(p[len]);				//输出内容
	if(-1 == munmap(p, sb.st_size))		//取消映射
	{
		perror("munmap");
		return 1;
	}
	return 0;
}


