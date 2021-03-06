#ifndef __TT_MSGQUEUE_H__
#define __TT_MSGQUEUE_H__

#include <pthread.h>
#include <time.h>

/* #include "tt_semaphore.h" use self defined semaphore */
// #include "tt_semaphore.h"
#ifndef __TT_SEMAPHORE_H__
	#include <semaphore.h>
	#define tt_sem_t sem_t
	#define tt_sem_init sem_init
	#define tt_sem_destroy sem_destroy
	#define tt_sem_getvalue sem_getvalue
	#define tt_sem_post sem_post
	#define tt_sem_wait sem_wait
	#define tt_sem_trywait sem_trywait
	#define tt_sem_timedwait sem_timedwait
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MSG_Q_ENTRY{
	void *content;
	struct MSG_Q_ENTRY *next;
}MSG_Q_ENTRY;

typedef struct MSG_Q{
	tt_sem_t get_sem;
	tt_sem_t put_sem;
	pthread_mutex_t lock;
	long int maxlen;
	struct MSG_Q_ENTRY *head;
	struct MSG_Q_ENTRY *tail;
}MSG_Q;

extern int msgq_init(MSG_Q *msg_q, long int maxlen);
extern int msgq_destroy(MSG_Q *msg_q);
extern int msg_put(MSG_Q *msg_q, void *entry);
extern int msg_tryput(MSG_Q *msg_q, void *entry);
extern void *msg_get(MSG_Q *msg_q);
extern void *msg_tryget(MSG_Q *msg_q);
extern void *msg_timedget(MSG_Q *msg_q, const struct timespec *timeout);
extern int msg_getvalue(MSG_Q *msg_q, long int *len);

#ifdef __cplusplus
}
#endif

#endif
