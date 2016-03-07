// #include <stdlib.h>

#include "threads.h"
#include "config.h"

#define THREAD_PSP	0xFFFFFFFD

typedef struct
{
	void *stack_start;
	void *stack_p;
	char active;
} thread_t;

static thread_t tasks[MAX_TASKS];
static int last_task_id;
static int first = 1;

int thread_create(void (*thread)(void *), void *data)
{
	int thread_id;
	void *stack_start;

	// Find a free thread
	for (thread_id = 0; thread_id < MAX_TASKS; thread_id++)
	{
		if (tasks[thread_id].active == 0)
			break;
	}

	if (thread_id == MAX_TASKS)
		return -1;

	// Create the stack
	stack_start = malloc(STACK_SIZE * sizeof(uint32_t));

	if (stack_start == 0)
		return -1;

	tasks[thread_id].stack_start = stack_start;

	// End of stack, minus what we are about to push
	stack_start += STACK_SIZE - 32;

	if (first)
	{
		stack_start[8] = (unsigned int) thread;
		stack_start[9] = (unsigned int) data;
		first = 0;
	}
	else
	{
		stack_start[8] = (unsigned int) THREAD_PSP;
		stack_start[9] = (unsigned int) data;
		stack_start[14] = (unsigned) &thread_self_destroy;
		stack_start[15] = (unsigned int) thread;

		// PSR Thumb bit
		stack_start[16] = (unsigned int) 0x21000000;
	}

	tasks[thread_id].stack_p = stack_start;
	tasks[thread_id].active = 1;

	return 0;
}

void thread_destroy(int thread_id)
{
	tasks[thread_id].active = 0;

	// Free the stack
	free(tasks[thread_id].stack_start);
}

void thread_self_destroy(void)
{
	asm volatile("cpsid i\n");
	thread_destroy(last_task_id);
	asm volatile("cpsie i\n");

	while (1);
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
