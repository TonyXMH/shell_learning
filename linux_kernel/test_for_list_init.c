/*************************************************************************
	> File Name: test_for_list_init.c
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月19日 星期三 18时26分44秒
 ************************************************************************/

#include<stdio.h>
//双向链表
struct list_head				//链表地址
{
	struct list_head *prev, *next;
};
//两种不同的初始化方法
#define LIST_HEAD_INIT(name) {&(name), &(name)}
#define LIST_HEAD(name)	\
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->prev = list;
	list->next = list;
}
int main(void)
{
	LIST_HEAD(temp1);
	printf("the address of prev :%x.\nthe address of next :%x\n", temp1.prev, temp1.next);
	struct list_head temp2;
	INIT_LIST_HEAD(&temp2);
	printf("the address of prev :%x.\nthe address of next :%x\n", temp2.prev, temp2.next);
	return 0;
}
