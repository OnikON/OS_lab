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

#define B_task
#define C_task

#ifdef C_task
	const int GLOBAL = 4;
#endif

int main() {
#ifdef A_task
	pthread_t tid;
#endif

#ifdef B_task
	pthread_t tid[5];
#endif
	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

#ifdef A_task
	err = pthread_create(&tid, NULL, mythread, NULL);
	if (err) {
		handle_error_en(err, "pthread_create");
	}
	err = pthread_join(tid, NULL);
	if (err) {
    	handle_error_en(err, "pthread_join");
	}
#endif

#ifdef B_task
	for (int N_thred = 0; N_thred < 5; N_thred++) {
		err = pthread_create(&tid[N_thred], NULL, mythread, NULL);
		if (err) {
	    	handle_error_en(err, "pthread_create");
		}
#ifdef C_task
		/*int errV = pthread_join(tid[N_thred], NULL);
		if (errV) {
	    	handle_error_en(errV, "pthread_join");
		}*/
		printf("    %s : %ld\n", __func__, tid[N_thred]);
#endif
	}
#endif

	pthread_exit(NULL);

}

//thread fun...
void *mythread(void *arg) {
	//for (int i = 0; i < 100; i++)
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());

#ifdef C_task
	printf("%s : %ld\n", __func__, pthread_self());
	int a = 5;
	const int b = 23;
	printf("%s : int a = %p\tconst int b = %p\tglobal = %p\n", __func__, &a, &b, &GLOBAL);
#endif
	return NULL;
}
