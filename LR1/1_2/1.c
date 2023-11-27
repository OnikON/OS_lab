#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define PARAMETRS struct parametrs
struct parametrs {
  int vale;
};

void *TreadMain(void *arg);	

int main (void) {

    printf("start\n");

    int ProgramStatus = 0;
    pthread_t thread;

    ProgramStatus = pthread_create(&thread, NULL, TreadMain, NULL);
    if (ProgramStatus) {
	    handle_error_en(ProgramStatus, "pthread_create");
	}

    ProgramStatus = pthread_join(thread, NULL);
    if (ProgramStatus) {
	    handle_error_en(ProgramStatus, "pthread_join");
	}
    
}

void *TreadMain(void *arg) {
    
    printf("thread_tid = %d\n", gettid());

}