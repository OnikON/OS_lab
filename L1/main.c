#include <stdio.h>
#include <pthread.h>

void print (char *line);

void* TreadMain (void* parameters);

int main (void) {
    int ProgramStatus = 0;
    pthread_t thread;
    //int createThreadStatus = pthread_create(&thread, NULL, TreadMain, NULL);
    ProgramStatus = pthread_create(&thread, NULL, TreadMain, NULL);
    if ( ProgramStatus != 0) {
        printf("I can't create a thread erro [ '%d' ]", ProgramStatus);
    }
    if (ProgramStatus == 0) {
        print("1 thread");
        pthread_exit(NULL);
    }
    return ProgramStatus;
}

void print (char *line) {
    for (int i = 0; i < 10; i++) {
        printf("%s : hi UwU\n", line);
    }
}

void* TreadMain (void* parameters) {
    print("2 thread");
}