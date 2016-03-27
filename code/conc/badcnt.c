/* 
 * badcnt.c - An improperly synchronized counter program 
 */
/* $begin badcnt */
#include "csapp.h"

void *thread(void *vargp);  /* Thread routine prototype */

/* Global shared variable */
volatile int cnt = 0; /* Counter */
sem_t *mutex;

int main(int argc, char **argv) 
{
	int niters;
	pthread_t tid1, tid2;

	mutex = sem_open("semaphore", O_CREAT, 0644, 1);

	/* Check input argument */
	if (argc != 2) { 
		printf("usage: %s <niters>\n", argv[0]);
		exit(0);
	}
	niters = atoi(argv[1]);

	/* Create threads and wait for them to finish */
	Pthread_create(&tid1, NULL, thread, &niters);
	Pthread_create(&tid2, NULL, thread, &niters);
	Pthread_join(tid1, NULL);
	Pthread_join(tid2, NULL);

	/* Check result */
	if (cnt != (2 * niters))
		printf("BOOM! cnt=%d\n", cnt);
	else
		printf("OK cnt=%d\n", cnt);

	exit(0);
}

/* Thread routine */
void *thread(void *vargp) 
{
	int i, niters = *((int *)vargp);

	for (i = 0; i < niters; i++) //line:conc:badcnt:beginloop
	{
		P(mutex);
		cnt++;                   //line:conc:badcnt:endloop
		V(mutex);
	}

	return NULL;
}
/* $end badcnt */

