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

#ifdef CTASK
	int GLOBAL = 4;
#endif

int main() {
#ifdef ATASK
	pthread_t tid;
#endif

#ifdef BTASK
	pthread_t tid[5];
#endif
	int err;
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
	scanf("%*d");
	
#ifdef ATASK
	err = pthread_create(&tid, NULL, mythread, NULL);
	if (err) {
		handle_error_en(err, "pthread_create");
	}
	//pthread_detach(tid);
	err = pthread_join(tid, NULL);
	if (err) {
    	handle_error_en(err, "pthread_join");
	}
#endif

#ifdef BTASK
	for (int N_thred = 0; N_thred < 5; N_thred++) {
		err = pthread_create(&tid[N_thred], NULL, mythread, NULL);
		if (err) {
	    	handle_error_en(err, "pthread_create");
		}
		sleep(5);
		//pthread_join(tid[N_thred], NULL);
		
#ifdef CTASK
		// int errV = pthread_join(tid[N_thred], NULL);
		// if (errV) {
	    // 	handle_error_en(errV, "pthread_join");
		// }
		
		printf("    %s : %ld\n\n", __func__, tid[N_thred]);
#endif
	}
	pthread_join(tid[4], NULL);
	// for (int N_thred = 0; N_thred < 5; N_thred++) {
	// 	int errV = pthread_join(tid[N_thred], NULL);
	// 	if (errV) {
	//     	handle_error_en(errV, "pthread_join");
	// 	}
	// }

#endif
	//sleep(1);
	
	pthread_exit(NULL);

}

//thread fun...
void *mythread(void *arg) {
	printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
	scanf("%*d");
	//sleep(2);
#ifdef CTASK
	printf("\n%s pthread_self: %ld\n", __func__, pthread_self());
	int a = 5;
	const int b = 23;	
	printf("\n%s : pointers :\n\tint a = %p\n\tconst int b = %p\n\tglobal = %p\n", __func__, &a, &b, &GLOBAL);
#endif

#ifdef DTASK
	a++;
	GLOBAL++;
	printf("%s : int a = %d\tglobal = %d\n", __func__, a, GLOBAL);

#endif
	return NULL;
}
