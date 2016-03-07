
#include "threads.h"

void first_thread(void *data)
{

}

void second_thread(void *data)
{

}

void third_thread(void *data)
{

}

int main(void)
{

  thread_create(first_thread, (void *)0);
  thread_create(second_thread, (void *)0);
  thread_create(third_thread, (void *)0);

  threads_run();

  return 0;
}
