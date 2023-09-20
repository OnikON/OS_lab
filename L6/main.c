#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#ifdef TIME
#include <time.h>
#endif

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define PARAMETRS struct parametrs
struct parametrs {
  char *str;
};

void print(char *argv);
void *TreadMain(void *parameters);

int main(int argc, char **argv) {
  int ProgramStatus = 0;
  pthread_t thread[argc - 1];

  PARAMETRS param[argc-1];
#ifdef TIME
  clock_t begin;
#endif

  for (int i = 0; i < (argc-1); i++) {
    param[i].str = argv[i+1];

    ProgramStatus = pthread_create(&thread[i], NULL, TreadMain, &param[i]);
    if (ProgramStatus != 0) {
    handle_error_en(ProgramStatus, "pthread_create");
    }

  }

#ifdef TIME
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("[main]sec - %f\n\n", time_spent);
#endif
  if (ProgramStatus == 0) {
    pthread_exit(NULL);
  }
  return ProgramStatus;
}

void print(char *str) {
  usleep(20000 * strlen(str));
  printf("%s\n", str);
}

void *TreadMain(void *parameters) {
#ifdef TIME
  clock_t begin;
#endif

  PARAMETRS *param = (PARAMETRS *)parameters;
  print(param->str);

#ifdef TIME
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("[thread]sec - %f\n\n", time_spent);
#endif
}
