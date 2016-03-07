// #include <stdlib.h>

#include "threads.h"
#include "config.h"

typedef struct {
	void *stack_start;
	void *stack_p;
	char active;
} thread_t;

static thread_t tasks[MAX_TASKS];
static int last_task_id;
static int first = 1;

int thread_create(void (*thread)(void *), void *data)
{
	return 0;
}

void thread_destroy(int thread_id)
{
	tasks[thread_id].active = 0;

	// Free the stack
	free(tasks[thread_id].stack_start);
}

void threads_run(void)
{
	last_task_id = 0;

	// Save kernel context
	asm volatile("mrs ip, psr\n"
	             "push {r4-r11, ip, lr}\n");

	// Move the task's stack pointer address into r0.
	asm volatile("mov r0, %0\n" : : "r" (tasks[last_task_id].stack_p));

	// Load user task's context and jump to the task
	asm volatile("msr psp, r0\n"
	             "mov r0, #3\n"
	             "msr control, r0\n"
	             "isb\n"
	             "pop {r4-r11, lr}\n"
	             "pop {r0}\n"
	             "bx lr\n");
}
