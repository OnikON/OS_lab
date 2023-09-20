#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4

void print(char **argv, int InedxS, int IndexE);
void *TreadMain(void *parameters);
void logick(int *mass, int argc);

#define PARAMETRS struct parametrs
struct parametrs {
  int start_index;
  int end_index;
  char **argv;
};

int main(int argc, char **argv) {
  int ProgramStatus = 0;
  pthread_t thread[NUM_THREADS];
  int logickmass[NUM_THREADS];
  logick(logickmass, argc);
  // (argc / 4) : (argc % 4) logick
  PARAMETRS param[NUM_THREADS];
  for (int n_thread = 0; n_thread < NUM_THREADS; n_thread++) {

    if (n_thread == 0) {
      param[n_thread].start_index = 1;
      param[n_thread].end_index = logickmass[n_thread];
    } else {
      param[n_thread].start_index = logickmass[n_thread - 1] + 1;
      param[n_thread].end_index = logickmass[n_thread];
    }

    param[n_thread].argv = argv;

    ProgramStatus =
        pthread_create(&thread[n_thread], NULL, TreadMain, &param[n_thread]);
    if (ProgramStatus != 0) {
      printf("I can't create a [%d]thread erro [ '%d' ]", n_thread,
             ProgramStatus);
    }
    #ifdef TEST
    ProgramStatus = pthread_join(thread[n_thread], NULL);
    #endif
  }

  if (ProgramStatus == 0) {
    pthread_exit(NULL);
  }
  return ProgramStatus;
}

void print(char **argv, int InedxS, int IndexE) {
  for (int i = InedxS; i < IndexE; i++) {
    printf("[%d]%s\n", i, argv[i]);
  }
}

void *TreadMain(void *parameters) {
  PARAMETRS *param = (PARAMETRS *)parameters;
  print(param->argv, param->start_index, param->end_index);
}

void logick(int *mass, int argc) {
  argc--;
  int withouе_trace = argc / NUM_THREADS;
  int trace = argc % NUM_THREADS;
  for (int i = 0; i < NUM_THREADS; i++) {
    if (trace > 0) {
      mass[i] = (withouе_trace * (i + 1)) + 1;
      trace--;
    } else {
      mass[i] = (withouе_trace * (i + 1));
    }
#ifdef TEST
    printf("mass[%d] = %d \n", i, mass[i]);
#endif
  }
}