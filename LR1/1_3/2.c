
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>


// Глобальный счетчик
volatile int counter = 0;

#define handle_error_en(en, msg)                                               \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void *thread_function(void *arg);

int main() {
    pthread_t thread;
    int ProgramStatus = 0;

    // Создание потока
    printf("%d\n", __LINE__);
    ProgramStatus = pthread_create(&thread, NULL, thread_function, NULL);
    if (ProgramStatus) {
        handle_error_en(ProgramStatus, "pthread_create");
    }

    // Ждем некоторое время, чтобы дать потоку выполниться
    sleep(5);

    // Отмена потока с использованием pthread_cancel()
    printf("%d\n", __LINE__);
    ProgramStatus = pthread_cancel(thread);
    if (ProgramStatus) {
        handle_error_en(ProgramStatus, "pthread_cancel");
    }

    // Ожидание завершения потока
    printf("%d\n", __LINE__);
    ProgramStatus = pthread_join(thread, NULL);
    printf("%d\n", ProgramStatus);
    if (ProgramStatus) {
        handle_error_en(ProgramStatus, "pthread_join");
    }
    printf("%d\n", __LINE__);
    printf("Main thread is done. Counter value: %d\n", counter);

    return ProgramStatus;
}

void *thread_function(void *arg) {
    
     while (1) {
        // Увеличиваем счетчик
        counter++;
        //printf("  !");
        //sleep(1);
    }

    pthread_exit(NULL);
}