
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>
#include <stdlib.h>


// Глобальный счетчик
volatile int counter = 0;
volatile int flag = 0;
volatile char* LINE;

#define handle_error_en(en, msg)                                               \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void *thread_function(void *arg);
void cleanup_function(void *arg);

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
    sleep(1);

    // Отмена потока с использованием pthread_cancel()
    printf("%d\n", __LINE__);
    ProgramStatus = pthread_cancel(thread);
    if (ProgramStatus) {
        handle_error_en(ProgramStatus, "pthread_cancel");
    }
    
    //flag = 1;
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
    //while (!flag){
    char* line = malloc(12 * sizeof(char));

    strcpy(line, "Hello World");

    pthread_cleanup_push(cleanup_function, line);
    printf("%p\n",line);

    while (!flag) {
        // Увеличиваем счетчик
        //counter++;
        printf("%s\n", line);
        //sleep(1);
    }

    //free(line);
    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}

void cleanup_function(void *arg) {
    char *message = (char *)arg;
    printf("%s: ", __func__);
    printf("%p\n", message);
    // Освобождаем память, выделенную под строку
    if (message != NULL) {
        free(message);
        message = NULL;
    }
}