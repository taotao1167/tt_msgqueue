#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "tt_msgqueue.h"

/*
------------|--------------|-------------------|--------------|------------------------|
        put       put       put                      put                      put         
----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
   get       try       try       tmd            tmd            tmd                 get    
*/

MSG_Q g_msgq;

const char *gettimestamp(){
	static char strbuf[20];
	struct timeval ts;
	gettimeofday(&ts, NULL);
	sprintf(strbuf, "[%ld.%06ld]", ts.tv_sec, ts.tv_usec);
	return strbuf;
}

void *thread1_func(void *para) {
	char *pcontent = NULL;

	sleep(2);
	pcontent = (char *)malloc(20);
	sprintf(pcontent, "msg %s", gettimestamp());
	printf("%s msg_put: \"%s\"\n", gettimestamp(), pcontent);
	msg_put(&g_msgq, pcontent);

	sleep(2);
	pcontent = (char *)malloc(20);
	sprintf(pcontent, "msg %s", gettimestamp());
	printf("%s msg_put: \"%s\"\n", gettimestamp(), pcontent);
	msg_put(&g_msgq, pcontent);

	sleep(2);
	pcontent = (char *)malloc(20);
	sprintf(pcontent, "msg %s", gettimestamp());
	printf("%s msg_put: \"%s\"\n", gettimestamp(), pcontent);
	msg_put(&g_msgq, pcontent);

	sleep(5);
	pcontent = (char *)malloc(20);
	sprintf(pcontent, "msg %s", gettimestamp());
	printf("%s msg_put: \"%s\"\n", gettimestamp(), pcontent);
	msg_put(&g_msgq, pcontent);

	sleep(5);
	pcontent = (char *)malloc(20);
	sprintf(pcontent, "msg %s", gettimestamp());
	printf("%s msg_put: \"%s\"\n", gettimestamp(), pcontent);
	msg_put(&g_msgq, pcontent);

	pthread_exit(NULL);
}

void *thread2_func(void *para) {
	char *pcontent = NULL;
	struct timespec timeout;
	timeout.tv_sec = 2;
	timeout.tv_nsec = 0;

	sleep(1);
	printf("%s call msg_get\n", gettimestamp());
	pcontent = msg_get(&g_msgq);
	if (pcontent != NULL) {
		printf("%s msg_get: %p, \"%s\"\n", gettimestamp(), pcontent, pcontent);
	} else {
		printf("%s msg_get: %p, \"null\"\n", gettimestamp(), pcontent);
		free(pcontent);
	}

	sleep(1);
	printf("%s call msg_tryget\n", gettimestamp());
	pcontent = msg_tryget(&g_msgq);
	if (pcontent != NULL) {
		printf("%s msg_tryget: \"%s\"\n", gettimestamp(), pcontent);
	} else {
		printf("%s msg_tryget: \"null\"\n", gettimestamp());
		free(pcontent);
	}

	sleep(2);
	printf("%s call msg_tryget\n", gettimestamp());
	pcontent = msg_tryget(&g_msgq);
	if (pcontent != NULL) {
		printf("%s msg_tryget: \"%s\"\n", gettimestamp(), pcontent);
	} else {
		printf("%s msg_tryget: \"null\"\n", gettimestamp());
		free(pcontent);
	}

	sleep(2);
	printf("%s call msg_timedget\n", gettimestamp());
	pcontent = msg_timedget(&g_msgq, &timeout);
	if (pcontent != NULL) {
		printf("%s msg_timedget: \"%s\"\n", gettimestamp(), pcontent);
	} else {
		printf("%s msg_timedget: \"null\"\n", gettimestamp());
		free(pcontent);
	}

	sleep(3);
	printf("%s call msg_timedget\n", gettimestamp());
	pcontent = msg_timedget(&g_msgq, &timeout);
	if (pcontent != NULL) {
		printf("%s msg_timedget: \"%s\"\n", gettimestamp(), pcontent);
	} else {
		printf("%s msg_timedget: \"null\"\n", gettimestamp());
		free(pcontent);
	}

	sleep(2);
	printf("%s call msg_timedget\n", gettimestamp());
	pcontent = msg_timedget(&g_msgq, &timeout);
	if (pcontent != NULL) {
		printf("%s msg_timedget: \"%s\"\n", gettimestamp(), pcontent);
	} else {
		printf("%s msg_timedget: \"null\"\n", gettimestamp());
		free(pcontent);
	}

	sleep(2);
	printf("%s call msg_get\n", gettimestamp());
	pcontent = msg_get(&g_msgq);
	if (pcontent != NULL) {
		printf("%s msg_get: \"%s\"\n", gettimestamp(), pcontent);
	} else {
		printf("%s msg_get: \"null\"\n", gettimestamp());
		free(pcontent);
	}

	pthread_exit(NULL);
}
int main() {
	int i = 0;
	int ret = 0;
	char *pcontent = NULL;

	msgq_init(&g_msgq, 3);
	for (i = 0; i < 10; i++) {
		pcontent = (char *)malloc(20);
		printf("before msg_put\n");
		ret = msg_tryput(&g_msgq, pcontent);
		printf("after msg_put, ret %d\n", ret);
	}
	return 0;
	pthread_t thread1 = 0, thread2 = 0;
	msgq_init(&g_msgq, 0);
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
	msgq_destroy(&g_msgq);
	printf("exit main()\n");
	return 0;
}
