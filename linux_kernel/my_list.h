/*************************************************************************
	> File Name: my_list.h
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月19日 星期三 22时17分08秒
 ************************************************************************/
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
//核心添加节点
static inline void __list_add(struct list_head *new, struct list_head *prev, list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
//头插法
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}
//尾插法
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}



