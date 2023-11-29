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
    printf("Thread ID: %ld\n", (long)pthread_self());
    pthread_exit(NULL);
}

int main() {
    int ProgramStatus = 0;
    pthread_t thread;
    
    while (1) {
        ProgramStatus = pthread_create(&thread, NULL, thread_function, NULL);
        if (ProgramStatus != 0) {
            handle_error_en(ProgramStatus, "pthread_create");
        }
        ProgramStatus = pthread_join(thread, NULL);
        if (ProgramStatus != 0) {
            handle_error_en(ProgramStatus, "pthread_join");
        }
        
        sleep(1);
    }

    return ProgramStatus;
}
