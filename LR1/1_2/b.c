#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define handle_error_en(en, msg)                                               \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void *thread_function(void *arg) {
    return (void*)42;
}

int main() {
    pthread_t thread;
    void *result;
    int ProgramStatus = 0;
    
    ProgramStatus = pthread_create(&thread, NULL, thread_function, NULL);
        if (ProgramStatus != 0) {
            handle_error_en(ProgramStatus, "pthread_create");
        }

    ProgramStatus = pthread_join(thread, &result);
        if (ProgramStatus != 0) {
            handle_error_en(ProgramStatus, "pthread_join");
        }

    printf("Main thread got result: %d\n", (int)result);

    return 0;
}
