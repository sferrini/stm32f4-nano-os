
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

  thread_create(first_thread, NULL);
  thread_create(second_thread, NULL);
  thread_create(third_thread, NULL);

  threads_run();

  return 0;
}
