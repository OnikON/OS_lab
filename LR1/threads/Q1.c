#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

void *mythread(void *arg);	

int main() {
	pthread_t tid;

	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
	scanf("%*d");
	
	err = pthread_create(&tid, NULL, mythread, NULL);
	if (err) {
		handle_error_en(err, "pthread_create");
	}
	pthread_detach(tid);
	err = pthread_join(tid, NULL);
	if (err) {
    	handle_error_en(err, "pthread_join");
	}

	//sleep(1);
	pthread_exit(NULL);

}

//thread fun...
void *mythread(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());

	return NULL;
}
