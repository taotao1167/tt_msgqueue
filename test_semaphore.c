#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "tt_semaphore.h"

/*
------------|--------------|-------------------|--------------|-------------------|
       post      post      post                     post                     post        
----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
  wait       try       try       tmd            tmd            tmd                wait   
*/

TT_SEM g_sem;

const char *gettimestamp() {
	static char strbuf[20];
	struct timeval ts;

	gettimeofday(&ts, NULL);
	sprintf(strbuf, "[%ld.%06ld]", ts.tv_sec, ts.tv_usec);
	return strbuf;
}

void *thread1_func(void *para) {
	sleep(2);
	printf("%s tt_sem_post\n", gettimestamp());
	tt_sem_post(&g_sem);

	sleep(2);
	printf("%s tt_sem_post\n", gettimestamp());
	tt_sem_post(&g_sem);

	sleep(2);
	printf("%s tt_sem_post\n", gettimestamp());
	tt_sem_post(&g_sem);

	sleep(5);
	printf("%s tt_sem_post\n", gettimestamp());
	tt_sem_post(&g_sem);

	sleep(5);
	printf("%s tt_sem_post\n", gettimestamp());
	tt_sem_post(&g_sem);

	pthread_exit(NULL);
}

void *thread2_func(void *para) {
	int ret = 0;
	struct timespec timeout;
	timeout.tv_sec = 2;
	timeout.tv_nsec = 0;

	sleep(1);
	printf("%s call tt_sem_wait\n", gettimestamp());
	ret = tt_sem_wait(&g_sem);
	printf("%s tt_sem_wait: %d\n", gettimestamp(), ret);

	sleep(1);
	printf("%s call tt_sem_trywait\n", gettimestamp());
	ret = tt_sem_trywait(&g_sem);
	printf("%s tt_sem_trywait: %d\n", gettimestamp(), ret);

	sleep(2);
	printf("%s call tt_sem_trywait\n", gettimestamp());
	ret = tt_sem_trywait(&g_sem);
	printf("%s tt_sem_trywait: %d\n", gettimestamp(), ret);

	sleep(2);
	printf("%s call tt_sem_timedwait\n", gettimestamp());
	ret = tt_sem_timedwait(&g_sem, &timeout);
	printf("%s tt_sem_timedwait: %d\n", gettimestamp(), ret);

	sleep(3);
	printf("%s call tt_sem_timedwait\n", gettimestamp());
	ret = tt_sem_timedwait(&g_sem, &timeout);
	printf("%s tt_sem_timedwait: %d\n", gettimestamp(), ret);

	sleep(2);
	printf("%s call tt_sem_timedwait\n", gettimestamp());
	ret = tt_sem_timedwait(&g_sem, &timeout);
	printf("%s tt_sem_timedwait: %d\n", gettimestamp(), ret);

	sleep(2);
	printf("%s call tt_sem_wait\n", gettimestamp());
	ret = tt_sem_wait(&g_sem);
	printf("%s tt_sem_wait: %d\n", gettimestamp(), ret);

	pthread_exit(NULL);
}

int main() {
	int ret = 0;
	pthread_t thread1 = 0, thread2 = 0;

	tt_sem_init(&g_sem, 0, 0);
	ret = pthread_create(&thread1, NULL, thread1_func, NULL);
	if (ret != 0) {
		printf("thread1 create failed.\n");
		return 0;
	}
	ret = pthread_create(&thread2, NULL, thread2_func, NULL);
	if (ret != 0) {
		printf("thread2 create failed.\n");
		return 0;
	}
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	tt_sem_destroy(&g_sem);
	printf("exit main()\n");
	return 0;
}
