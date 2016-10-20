/*************************************************************************
	> File Name: test_the_value_of_pointer.c
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月20日 星期四 13时08分03秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
	int intNum;
	printf("the address of an integer :%x\n", &intNum);		//以十六进制的方式输出。
	printf("the address of an integer :%d\n", (int)&intNum);		//以十六进制的方式输出。
	printf("the address of an integer :%ld\n", (long)&intNum);		//以十六进制的方式输出。
//	printf("the address of an integer :%d\n", &intNum);		//以十进制的方式输出。
//	printf("the address of an integer :%ld\n", &intNum);		//以十进制的方式输出。
	return 0;
}
