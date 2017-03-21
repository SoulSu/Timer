#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "timer.h"

#include "tools.c"

#ifndef ERROR
#define ERROR(s) printf("%s file %s line %d\n", s, __FILE__, __LINE__)
#endif

#ifndef TIME_SLEEP
#define TIME_SLEEP 1
#endif


// ADT
// 环形队列， timer 任务执行
// 具体方法实现

slot_linked *init_slot_linked(unsigned int count)
{
	if (2 > count)
	{
		ERROR("count max gt 2");
		exit(3);
	}

	slot_linked *_slot_linked = new_slot_linked(sizeof(slot_linked));
	// 先生成对应的链接
	_slot_linked->total = count;

	slot *_first = new_slot(sizeof(slot));
	_first->pos = 1;
	_first->prev = NULL;
	_first->name = (char *)malloc(sizeof(char) * 1);
	sprintf(_first->name, "%d", _first->pos);

	int i;
	slot *next;
	slot *tmp = _first;
	for (i = 1; i < count; ++i)
	{
		next = new_slot(sizeof(slot));
		next->pos = i + 1;
		next->prev = tmp;
		next->name = (char *)malloc(sizeof(char) * 1);
		sprintf(next->name, "%d", next->pos);


		tmp->next = next;
		tmp = next;
	}

	DEBUG_LOG("first %p %p %p\n", _first, tmp, tmp->prev);
	tmp->next = _first;
	_first->prev = tmp;

	_slot_linked->slot = _first;
	return _slot_linked;
}

slot_linked *new_slot_linked(size_t size)
{
	slot_linked *tmp = (slot_linked *) malloc(size);
	if (NULL == tmp)
	{
		ERROR("malloc new_slot_linked error");
		exit(1);
	}
	return tmp;
}

slot *new_slot(size_t size)
{
	slot *tmp = (slot *) malloc(size);
	if (NULL == tmp)
	{
		ERROR("malloc new_slot error");
		exit(2);
	}
	return tmp;
}

void p_slot_linked(slot_linked *l)
{
	printf("%s\t%10s\t%10s\t%10s\t%10s\n", "pos", "name", "s[p]", "n[p]", "p[p]");
	slot *next = l->slot;
	int i =1;
	do{
		printf("%d\t%10s\t%10p\t%10p\t%10p\n", next->pos, next->name, next, next->next, next->prev);
	}while( (next = next->next) != NULL && i++ < l->total);

}

bool run(slot_linked *slot_linked)
{
	unsigned int times = 1;
	slot *slot = slot_linked->slot;

	while(true){
		sleep(TIME_SLEEP);
		// todo 拉一个线程出去单独执行槽中的定时任务
		DEBUG_LOG("timer slot %d ", times);
		if(times++ >= slot_linked->total)
		{
			times = 1;
		}
		run_task(slot, slot_linked->total);
		slot = slot->next;
	}
}

bool run_task(slot *slot, const long total)
{
	// 遍历timer 并对比时间是否执行该timer
	if( NULL ==slot->timer )
	{
		// 没有任务
		DEBUG_LOG("\n");
		return true;
	}
	else
	{
		DEBUG_LOG("task num {%d} address {%p}\n", slot->timer->total, slot->timer);
		// 有任务存在 - 遍历队列 判断是否当前时间执行方法
		timer *_exec_timer = slot->timer->timer;

		do
		{
			if (_exec_timer->_next_exec_num_tikect-- == 0)
			{
				_exec_timer->exec_times --;
				DEBUG_LOG("run task name {%s} left exec_times {%d}\n", _exec_timer->name, _exec_timer->exec_times);
				_exec_timer->_next_exec_num_tikect = (long) (total / _exec_timer->next_exec_time);;
			}

			// 没有执行次数 回收资源
			if(_exec_timer->exec_times <= 0)
			{
				// 只有一个任务
				if(slot->timer->total-- == 1)
				{
					// free(slot->timer->timer);
					slot->timer = NULL;
				}
				else
				{
					// 说明是最后一个
					if (_exec_timer->next == NULL)
					{
						_exec_timer->prev->next = NULL;
					}
					else if (_exec_timer->prev == NULL)
					{
						// 说明是第一个
						_exec_timer->next->prev = NULL;
						slot->timer->timer = _exec_timer->next;
					}
					else
					{
						// 说明在中间
						_exec_timer->prev->next = _exec_timer->next;
					}
				}
				// free(_exec_timer);
			}
		} while ((_exec_timer = _exec_timer->next) != NULL);
	

	}

	return true;
}

bool add_task(slot_linked *slot_linked, timer *task_timer)
{
	// 添加一个任务执行， 自己根据当前时间加入到当前运行的下一个槽中，并且自动hash 一个值保存
	char *name = task_timer->name;
	int add_pos = ( task_timer->add_time % slot_linked->total ) + 1;

	// 初始化下次执行技术器
	// slot_linked->total
	task_timer->_next_exec_num_tikect = (long) (slot_linked->total / task_timer->next_exec_time);

	DEBUG_LOG("task {%s} add into pos {%d}\n", name, add_pos);

	slot *slot = slot_linked->slot;

	do
	{
		// 找到了要添加的槽
		if (slot != NULL && slot->pos == add_pos)
		{
			timer_linked *_timer_linked = slot->timer;
			
			if (NULL == _timer_linked)
			{
				_timer_linked = (timer_linked *) malloc(sizeof(timer_linked));

				timer *first_timer = task_timer;
				first_timer->prev = NULL;
				first_timer->next = NULL;
				_timer_linked->timer = first_timer;
				slot->timer = _timer_linked;
			}
			else
			{

				timer *tmp_timer = _timer_linked->timer;
				task_timer->prev = NULL;
				task_timer->next = tmp_timer;

				tmp_timer->prev = task_timer;

				slot->timer = _timer_linked;
			}
			
			_timer_linked->total++;

			break;
		}

		slot = slot->next;
	} while(true);

	return true;

}
