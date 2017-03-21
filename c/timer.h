#include <stdio.h>


typedef enum
{
	false,
	true,
} bool;

// timer 任务主体
typedef struct timer
{
	char *hash;// 生成的唯一hash值 用于持久化
	char *name;// timer 名称

	long add_time; // 加入时间
	long next_exec_time; // 下次执行时间
	long _next_exec_num_tikect; // 下次执行计数

	int exec_times; // 执行次数

	struct timer *prev; // 前面一个任务
	struct timer *next; // 下一个任务
}timer;

// 时间任务主体 链表结构 不用闭环
typedef struct 
{
	unsigned int total;

	timer *timer;
}timer_linked;

// 定义槽数据结构
typedef struct slot
{
	timer_linked *timer; // 任务
	
	char *name; // 槽名称

	unsigned int pos; // 偏移量

	struct slot *prev; // 构成环形
	struct slot *next; // 构成环形
} slot;


// 定义槽链表
typedef struct 
{
	unsigned int total; // 槽的个数， 初始化是按照 秒 进行位移

	slot *slot; // 槽
}slot_linked;


slot_linked *init_slot_linked(unsigned int); // 初始化槽
slot_linked *new_slot_linked(size_t size);
slot *new_slot(size_t size);
void p_slot_linked(slot_linked *); // 打印
bool run(slot_linked *); // 执行队列主方法体
bool run_task(slot *,const long); // 执行队列主方法体
bool add_task(slot_linked *, timer *); // 添加一个任务执行， 自己根据当前时间加入到当前运行的下一个槽中，并且自动hash 一个值保存



