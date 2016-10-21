/*************************************************************************
	> File Name: my_list.h
	> Author: tony
	> Mail: 445241843@qq.com 
	> Created Time: 2016年10月19日 星期三 22时17分08秒
 ************************************************************************/
#define LIST_POISON1 ((void *)0x00100100)		//保证链表中的节点项不可访问，对LIST_POISON1和对LIST_POISON2的访问都将引起页故障？？？？
#define LIST_POISON2 ((void *)0x00200200)		//这之后可以在看page.h的中找一下原因
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


//删除的核心算法(非常的简洁)
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}
//删除节点（封装了上述函数）对要删除的节点不做任何操作
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = LIST_POISON1;				//这里暂且先不讨论它为什么不将2个指针赋值为NULL
	entry->prev = LIST_POISON2;
}

//对删除的节点初始化很一个新的链表
static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);					//这里用的是内联函数而不是宏函数，参数是有类型的。
}
//移动节点
//以头插法添加节点
static inline void list_move(struct list_head *list, struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}
//移动节点
//以尾插法添加节点
static inline void list_move_tail(struct list_head *list, struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}
//替换节点,我们没有对旧的节点做任何操作
static inline void list_replace(struct list_head *old, struct list_head *new)
{
	new->next = old->next;
	new-prev = old->prev;
	new->next->prev = new;
	new->prev->next = new;
}
//替换节点,将旧节点初始化为新的链表
static inline void list_replace_init(struct list_head *old, struct list_head *new)
{
	list_replace(old, new);
	INIT_LIST_HEAD(old);
}


