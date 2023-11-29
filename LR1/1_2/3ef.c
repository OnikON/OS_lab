#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>

#define NUM_THREADS 1

#define handle_error_en(en, msg)                                               \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void *TreadMain(void *arg);

int main(void) {

  printf("start\n");

  int ProgramStatus = 0;
  pthread_t thread;
  pthread_attr_t attr;

  printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
  scanf("%*d");

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  while (1) {
    
    ProgramStatus = pthread_create(&thread, &attr, TreadMain, NULL);
    if (ProgramStatus) {
      handle_error_en(ProgramStatus, "pthread_create");
    }

    // ProgramStatus = pthread_detach(thread);
    // if (ProgramStatus) {
    //   handle_error_en(ProgramStatus, "pthread_detach");
    // }

    // ProgramStatus = pthread_join(thread, NULL);
    // if (ProgramStatus) {
    //   handle_error_en(ProgramStatus, "pthread_join");
    // }
  }

  return ProgramStatus;
}

void *TreadMain(void *parameters) {
  printf("in thread tid = [%d]\n", gettid());
  return NULL;
}
