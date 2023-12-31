#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_THREADS 1

#define handle_error_en(en, msg)                                               \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

#define PARAMETRS struct parametrs
struct parametrs {
  int value;
};

void *TreadMain(void *arg);

int main(void) {

  printf("start\n");

  int ProgramStatus = 0;
  pthread_t thread;
  PARAMETRS param[NUM_THREADS];

  param->value = 666;

  printf("in main start value = [%d]\n", param->value);

  ProgramStatus = pthread_create(&thread, NULL, TreadMain, &param[0]);
  if (ProgramStatus) {
    handle_error_en(ProgramStatus, "pthread_create");
  }

  ProgramStatus = pthread_join(thread, NULL);
  if (ProgramStatus) {
    handle_error_en(ProgramStatus, "pthread_join");
  }

  printf("in main value = [%d]\n", param->value);

  return ProgramStatus;
}

void *TreadMain(void *parameters) {
  PARAMETRS *param = (PARAMETRS *)parameters;
  param->value = 42;
  printf("in thread value = [%d]\n", param->value);
}

void print(int value) { printf("in thread value = [%d]\n", value); }