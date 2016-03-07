
#ifndef __THREADS_HEADER__
#define __THREADS_HEADER__

int thread_create(void (*thread)(void *), void *data);
void thread_destroy(int thread_id);
void threads_run(void);

#endif // __THREADS_HEADER__
