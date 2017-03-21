#include <stdio.h>
#include <time.h>

#include "timer.c"


int main(int argc, char const *argv[])
{
	printf("%d\n", time(NULL));
	slot_linked *slot_linked = init_slot_linked(5);
	p_slot_linked(slot_linked);

	timer t1 = {
		name:"timer1",
		add_time:(long) time(NULL),
		exec_times: 2,
		next_exec_time: 5
	};
	// 添加任务
	add_task(slot_linked, &t1);

	sleep(1);
	timer t2 = {
		name:"timer2",
		add_time:(long) time(NULL),
		exec_times: 4,
		next_exec_time: 10
	};
	// 添加任务
	add_task(slot_linked, &t2);
	sleep(2);
	timer t3 = {
		name:"timer3",
		add_time:(long) time(NULL),
		exec_times: 2,
		next_exec_time: 3
	};
	// 添加任务
	add_task(slot_linked, &t3);
	sleep(2);
	timer t4 = {
		name:"timer4",
		add_time:(long) time(NULL),
		exec_times: 3,
		next_exec_time: 4
	};
	// 添加任务
	add_task(slot_linked, &t4);

sleep(2);
	timer t5 = {
		name:"timer5",
		add_time:(long) time(NULL),
		exec_times: 6,
		next_exec_time: 4
	};
	// 添加任务
	add_task(slot_linked, &t5);
	run(slot_linked);
	return 0;
}