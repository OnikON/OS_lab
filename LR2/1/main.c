#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include <string.h>

void HI(const char* mess) {
    printf("\tHI from %s, my tid{%s}\n", mess, gettid());

    return ((void)0);
}

void* thread_F(void* arg) {
    #ifdef MODE == 1
        prinf("MODE 1\n");
    #endif

    #ifdef MODE == 2

    #endif

    #ifdef MODE == 3

    #endif

    return ((void*)0);
}

int main(void) {
    pthread_t tid[3];

    #define MODE 1
    pthread_create(&tid[0], NULL, thread_F, NULL);

    int status = 0;

    HI(__func__);

    return status;
}