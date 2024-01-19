#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Глобальная переменная для использования в обработчиках сигналов
sigset_t sigset;

// Обработчик сигнала SIGINT
void sigint_handler(int signo) {
    printf("Thread 2: Received SIGINT\n");
    exit(0);
}

// Поток, который блокирует все сигналы
void* block_signals_thread(void* arg) {
    //sigset_t set;
    //sigfillset(&sigset);

    if (sigfillset(&sigset) == -1) {
         perror("block_signals_thread: sigfillset() failed");
         return NULL;
    }

    pthread_sigmask(SIG_SETMASK, &sigset, NULL);
    
    while (1) {
        // Бесконечный цикл для блокировки сигналов
        sleep(1);
    }
    
    return NULL;
}

// Поток, который принимает SIGQUIT при помощи функции sigwait()
void* sigquit_thread(void* arg) {
    int sig;
    
    printf("Thread 3: Waiting for SIGQUIT using sigwait()\n");
    
    // Ожидаем сигнал SIGQUIT
    sigwait(&sigset, &sig);
    
    // Обработка сигнала
    printf("Thread 3: Received SIGQUIT (%d)\n", sig);
    
    exit(0);
    
    return NULL;
}

int main() {
    pthread_t tid1, tid2, tid3;

    // Создаем первый поток для блокировки сигналов
    pthread_create(&tid1, NULL, block_signals_thread, NULL);
    
    // Устанавливаем обработчик сигнала SIGINT
    signal(SIGINT, sigint_handler);
    
    // Создаем второй поток для обработки SIGINT
    pthread_create(&tid2, NULL, block_signals_thread, NULL);

    // Создаем третий поток для ожидания SIGQUIT с использованием sigwait()
    pthread_create(&tid3, NULL, sigquit_thread, NULL);

    // Ожидаем завершения всех потоков
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    return 0;
}
