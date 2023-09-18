#include <stdio.h>

#define NUM_THREADS 4
#define B

#ifdef A
int main(int argc, char **argv) {
  printf("argc = %d\n", argc);
  for (int i = 0; i < argc; i++) {
    printf("[%d]%s\n", i, argv[i]);
  }
  return 0;
}
#endif

#ifdef B

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
  }
}

int main(int argc, char **argv) {
  int mass[4];
  logick(mass, argc);
  for (int i = 0; i < NUM_THREADS; i++) {
    printf("%d\n", mass[i]);
  }
}
#endif