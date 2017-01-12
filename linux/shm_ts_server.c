#include <stdio.h>
#include <sys/shm.h>
#include <sys/file.h>
#include <sys/sem.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define TIME_MEM_KEY  99
#define SEG_SIZE ((size_t)100)
#define TIME_SEM_KEY 9900
#define oops(m,x) {perror(m); exit(x);}

union semun {int val; struct semid_ds *buf; ushort *array;};
int seg_id, semset_id;
void cleanup(int);  //信号参数为int 型
void set_sem_value(int semset_id, int semum, int val);
void wait_and_lock(int semset_id);
void release_lock(int semset_id);

int main()
{
	char *mem_ptr, *ctime();
	time_t now;
	int n;

	seg_id = shmget(TIME_MEM_KEY, SEG_SIZE, IPC_CREAT|0777);
	if (seg_id == -1)
		oops("shget", -1);


	mem_ptr = shmat(seg_id, NULL, 0);
	if (mem_ptr == (void *)-1)
		oops("shmat", -1);

	semset_id = semget(TIME_SEM_KEY, 2, 0666|IPC_CREAT|IPC_EXCL);
	if (semset_id == -1)
		oops("semget", -1);
		
	set_sem_value(semset_id, 0, 0);
	set_sem_value(semset_id, 0, 0);

	signal(SIGINT, cleanup);

	for (n = 0; n < 60; n++)
	{
		time(&now);
			printf("\t shm_ts2 waiting for lock.\n");
		wait_and_lock(semset_id);
			printf("\t shm_ts2 updating memory.\n");
		strcpy(mem_ptr, ctime(&now));
			sleep(5);
		release_lock(semset_id);
			printf("\t shm_ts2 released lock.\n");
		sleep(1);
	}

}
void set_sem_value(int semset_id, int semum, int val)
{
	union semun initval;

	initval.val = val;
	if (semctl(semset_id, semum, SETVAL, initval) == -1)
		oops("semctl", 4);
}

void cleanup(int sig_num)
{
	shmctl(seg_id, IPC_RMID, NULL);
	semctl(semset_id, 0, IPC_RMID, NULL);
}

void wait_and_lock(int semset_id)
{
	struct sembuf actions[2];
	actions[0].sem_num = 0;
	actions[0].sem_flg = SEM_UNDO;
	actions[0].sem_op = 0;

	actions[1].sem_num = 1;
	actions[1].sem_flg = SEM_UNDO;
	actions[1].sem_op = +1;

	if (semop(semset_id, actions, 2) == -1)
		oops("semop", 10);
}

void release_lock(int semset_id)
{
	struct sembuf actions[1];
	actions[0].sem_num = 1;
	actions[0].sem_flg = SEM_UNDO;
	actions[0].sem_op = -1;

	if (semop(semset_id, actions, 1) == -1)
		oops("semop", 10);

}
