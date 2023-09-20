#include <pthread.h>
#include <stdio.h>
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
  int argc;
  char **argv;
};

void print(char **argv, int argc);
void *TreadMain(void *parameters);
void DergiVKurse ();

int main(int argc, char **argv) {
  int ProgramStatus = 0;
  pthread_t thread;

  PARAMETRS param;
  param.argv = argv;
  param.argc = argc;

  ProgramStatus = pthread_create(&thread, NULL, TreadMain, &param);
  if (ProgramStatus != 0) {
    handle_error_en(ProgramStatus, "pthread_create");
  }
#ifdef TIME
  clock_t begin;
#endif
  usleep(2);
  ProgramStatus = pthread_cancel(thread);
  if (ProgramStatus != 0) {
    handle_error_en(ProgramStatus, "pthread_cancel");
  }
  
  void *res;

  ProgramStatus = pthread_join(thread, &res);
    if ( ProgramStatus != 0) {
        handle_error_en(ProgramStatus, "pthread_join");
    }
  if (res == PTHREAD_CANCELED)
               printf("%s(): thread was canceled\n", __func__);
           else
               printf("%s(): thread wasn't canceled (shouldn't happen!)\n",
                      __func__);
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

void print(char **argv, int argc) {
  for (int i = 1; i < (argc); i++) {
    printf("[%d]%s\n", i, argv[i]);
  }
}

void *TreadMain(void *parameters) {
#ifdef TIME
  clock_t begin;
#endif

  pthread_cleanup_push(DergiVKurse, NULL);
  PARAMETRS *param = (PARAMETRS *)parameters;
  print(param->argv, param->argc);
  pthread_cleanup_pop(0);

#ifdef TIME
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("[thread]sec - %f\n\n", time_spent);
#endif
}

void DergiVKurse () {
  printf("\n\t\t[%s]:thread was be cancel\n", __func__);
}
