#define _GNU_SOURCE
#include <pthread.h>
#include <assert.h>

#include "queue.h"

void *qmonitor(void *arg) {
	queue_t *q = (queue_t *)arg;

	printf("qmonitor: [%d %d %d]\n", getpid(), getppid(), gettid());

	while (1) {
		queue_print_stats(q);
		sleep(1);
	}

	return NULL;
}

queue_t* queue_init(int max_count) {
	int err;

	queue_t *q = malloc(sizeof(queue_t));
	if (!q) {
		printf("Cannot allocate memory for a queue\n");
		abort();
	}

	q->first = NULL;
	q->last = NULL;
	q->max_count = max_count;
	q->count = 0;

	q->add_attempts = q->get_attempts = 0;
	q->add_count = q->get_count = 0;

	err = pthread_create(&q->qmonitor_tid, NULL, qmonitor, q);
	if (err) {
		printf("queue_init: pthread_create() failed: %s\n", strerror(err));
		abort();
	}

	err = pthread_spin_init(&q->spinlock, 0);
	if (err) {                    
        printf("queue_init: pthread_spin_init() failed: %s\n", strerror(err));
        abort();                  
    }

	return q;
}

void queue_destroy(queue_t *q) {
	// TODO: It's needed to implement this function
	int status = 0;
	status = pthread_cancel(q->qmonitor_tid);

	if (status) {
        fprintf(stderr, "queue_destroy(): pthread_cancel() failed: %s\n", strerror(status));
    }

	qnode_t* tmp = q->first;
	while (tmp != NULL) {
        qnode_t* next = tmp->next;
        free(tmp);
        tmp = next;
    }

	status = pthread_spin_destroy(&q->spinlock);
    if (status) {
        printf("queue_destroy: pthread_spin_destroy() failed: %s\n", strerror(status));
    }

	free(q);
}

int queue_add(queue_t *q, int val) {

	pthread_spin_lock(&(q->spinlock));

	q->add_attempts++;

	assert(q->count <= q->max_count);

	if (q->count == q->max_count) {
		pthread_spin_unlock(&(q->spinlock));
		return 0;
	}

	qnode_t *new = malloc(sizeof(qnode_t));
	if (!new) {
		printf("Cannot allocate memory for new node\n");
		abort();
	}

	new->val = val;
	new->next = NULL;
	//ошибка? это условии круто было бы проверить в самом начале, и обрбаотать вариант одновременного удаления и добавления, если разммер текущей очереди достаточно велик.
	if (!q->first)
		q->first = q->last = new;
	else {
		q->last->next = new;
		q->last = q->last->next;
	}

	q->count++;
	q->add_count++;

	pthread_spin_unlock(&(q->spinlock));

	return 1;
}

int queue_get(queue_t *q, int *val) {
	
	pthread_spin_lock(&(q->spinlock));

	q->get_attempts++;

	assert(q->count >= 0);

	if (q->count == 0) {
		pthread_spin_unlock(&(q->spinlock));
		return 0;
	}
	qnode_t *tmp = q->first;
	//ошибка? [если многоппоток, то может указывать на NULL, очередь не нулевая, ]
	*val = tmp->val;
	//ошибка? [если многоппоток, то может указывать на NULL, к очереди могут обращаться несколько потоков с запросом на извлечение]
	q->first = q->first->next;

	free(tmp);
	q->count--;
	q->get_count++;
	pthread_spin_unlock(&(q->spinlock));
	return 1;
}

void queue_print_stats(queue_t *q) {
	pthread_spin_lock(&(q->spinlock));
	printf("queue stats: current size %d; attempts: (%ld %ld %ld); counts (%ld %ld %ld)\n",
		q->count,
		q->add_attempts, q->get_attempts, q->add_attempts - q->get_attempts,
		q->add_count, q->get_count, q->add_count -q->get_count);
	pthread_spin_unlock(&(q->spinlock));
}

