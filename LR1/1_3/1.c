
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

void *thread_function(void *arg);

int main() {
    pthread_t thread;
    int ProgramStatus = 0;

    // Создание потока
    ProgramStatus = pthread_create(&thread, NULL, thread_function, NULL);
    if (ProgramStatus) {
        handle_error_en(ProgramStatus, "pthread_create");
    }

    // Ждем некоторое время, чтобы дать потоку выполниться
    sleep(3);

    // Отмена потока с использованием pthread_cancel()
    ProgramStatus = pthread_cancel(thread);
    if (ProgramStatus) {
        handle_error_en(ProgramStatus, "pthread_cancel");
    }

    // Ожидание завершения потока
    ProgramStatus = pthread_join(thread, NULL);
    if (ProgramStatus) {
        handle_error_en(ProgramStatus, "pthread_join");
    }

    printf("Main thread is done\n");

    return ProgramStatus;
}

void *thread_function(void *arg) {
    // В бесконечном цикле печатаем строки
    while (1) {
        printf("Thread is running\n");
        //sleep(1);
    }

    pthread_exit(NULL);
}