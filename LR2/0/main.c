#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <signal.h>

volatile int FLAG = 0;
volatile int t1 = 0;
volatile int t2 = 0;
volatile int t3 = 0;

volatile int p1 = 0;
volatile int p2 = 0;
volatile int p3 = 0;

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

void MyMenu() {
    system("clear");
    printf("thread[%d] 1 status = %d\n", p1, t1);
    sleep(1);
    printf("thread[%d] 2 status = %d\n", p2, t2);
    sleep(1);
    printf("thread[%d] 3 status = %d\n", p3, t3);
    sleep(1);
    // printf(".");
    // sleep(1);
    // printf(".");
    // sleep(1);
    // printf(".\n");
    // sleep(1);
}

void HI(const char* mess) {
    printf("\tHI from %s, my tid{%d}\n", mess, gettid());
    fflush(stdout);
    return ((void)0);
}

void where_are_you_from(const char* mess) {
    write(stdout, mess, strlen(mess));
    fsync(stdout);
    return ((void)0);
}

void print_blocked_signals() {
    sigset_t current_mask;
    if (pthread_sigmask(SIG_BLOCK, NULL, &current_mask) != 0) {
        perror("pthread_sigmask");
        exit(EXIT_FAILURE);
    }

    printf("Blocked signals: ");
    for (int sig = 1; sig <= SIGRTMAX; ++sig) {
        if (sigismember(&current_mask, sig)) {
            printf("%d ", sig);
        }
    }
    printf("\n");
}

void count1() {
    t1++;
} 
void count2() {
    t2++;
}
void count3() {
    t3++;
}
void* thread_F(void* arg) {
    //printf("\t\t\t TF\n");
    int func_mode = (void*) arg;
    int status = 0;

    if(func_mode == 1) {
        p1 = gettid();
        HI(__func__);
        sigset_t sig_mask;

        status = sigfillset(&sig_mask);
        if (status) {
	    	handle_error_en(status, "sigfillset() failed");
            return ((void*)status);
	    }

        status = pthread_sigmask(SIG_SETMASK, &sig_mask, NULL);
        if (status) {
	    	handle_error_en(status, "pthread_sigmask");
            return ((void*)status);
	    }

        while(t1 >= 0) {
            sleep(1);
            t1++;
        }

        return ((void*)status);

    }

    if(func_mode == 2) {
        p2 = gettid();
        HI(__func__);

        signal(SIGINT, count2);

        while(1) {
            sleep(1);
        }

        return ((void*)status);
    }

    if(func_mode == 3) {
        p3 = gettid();
        HI(__func__);
        sigset_t sig_mask;

        status = sigemptyset(&sig_mask);
        if (status) {
	    	handle_error_en(status, "sigemptyset() failed");
            return ((void*)status);
	    }

        status = sigaddset(&sig_mask, SIGQUIT);
        if (status) {
	    	handle_error_en(status, "sigaddset() failed");
            return ((void*)status);
	    }

        int sig;
        status = sigwait(&sig_mask, &sig);
        if (status) {
	    	handle_error_en(status, "sigwait() failed");
            return ((void*)status);
	    }
        // while(1) {
        //     sleep(1);
        // }
        printf("\tpiw-paw\n");
        exit(0);
        return ((void*)status);
    }

    return ((void*)status);
}

int main(void) {
    system("clear");
    pthread_t tid[3];
    int status = 0;

    status = pthread_create(&tid[0], NULL, thread_F, (void*)1);
    if (status) {
	    	handle_error_en(status, "pthread_create");
	}
    status = pthread_create(&tid[1], NULL, thread_F, (void*)2);
    if (status) {
	    	handle_error_en(status, "pthread_create");
	} 
    status = pthread_create(&tid[2], NULL, thread_F, (void*)3);
    if (status) {
	    	handle_error_en(status, "pthread_create");
	}
    //thread_F(NULL);
    

    HI(__func__);

    char cons = 1;
    while(cons != '4'){
        cons = getchar();
        pthread_kill(tid[2], SIGINT);
        if (cons == '0')
            exit(1);
        MyMenu();
    }

    status = pthread_kill(tid[2], SIGQUIT);
    if (status) {
	    	handle_error_en(status, "pthread_kill");
	}
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    return status;
}