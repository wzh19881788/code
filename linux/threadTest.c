#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct arg_set{
	char *fname;
	int count;
} *mailbox;

mailbox s_mailbox;
	
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;

void *count_words(void *a)
{
	mailbox mail = (mailbox)a;
	char c, prevc = '\0';
	
	FILE *fp = fopen(mail->fname, "r");
	if (fp == NULL)
		perror(mail->fname);
	else{
		while((c = getc(fp) )!= EOF)
		{
			if (!isalnum(c) && isalnum(prevc))
				mail->count ++;
			prevc = c;	
		}
		fclose(fp);
	}	
	printf("COUNT:finished waiting to get the lock.\n");
	pthread_mutex_unlock(&lock);
	if (s_mailbox != NULL)
		pthread_cond_wait(&flag, &lock);
	pthread_mutex_lock(&lock);
	printf("COUNT: have lock, waiting the signal.\n");
	s_mailbox = mail;
	pthread_cond_signal(&flag);
	printf("COUNT: raising the flag.\n");
	pthread_mutex_unlock(&lock);
	printf("COUNT: Unlock the lock\n");
	
	return NULL;
}

int main(int argc, char*argv[])
{
	pthread_t t1, t2;
	struct arg_set args1, args2;
	int reports_in = 0;
	int total_words = 0;

	if (argc != 3){
		printf("MAIN: the command should contain the file name.\n");
		exit(0);
	}

	pthread_mutex_lock(&lock);

	args1.fname = argv[1];
	args1.count = 0;
	pthread_create(&t1, NULL, count_words, (void *)&args1);

	
	args2.fname = argv[2];
	args2.count = 0;
	pthread_create(&t2, NULL, count_words, (void *)&args2);

	while(reports_in < 2){

	printf("MAIN: waiting for the singal arrived.\n");
	pthread_cond_wait(&flag, &lock);
	printf("MAIN: flag was raise.\n");
	total_words += s_mailbox->count;
	if (s_mailbox == &args1)
		pthread_join(t1, NULL);

	if (s_mailbox == &args2)
		pthread_join(t2, NULL);

	printf("MAIN: %s:%d\n", s_mailbox->fname, s_mailbox->count);	
	s_mailbox = NULL;

	pthread_cond_signal(&flag);
	reports_in ++;
	}

	printf("total words: %d\n", total_words);

	return 0;
}































