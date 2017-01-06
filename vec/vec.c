#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct node {
	int data;
	node *next;
}node;

node *create()
{
	int i = 0;
	node *head, *p, *q;
	int x = 0;
	head = (node*) malloc(sizeof(node));

	while(1)
	{
		printf("please input the data: ");
		scanf("%d", &x);
		if (x == 0)
			break;
		else{
			p = (node *)malloc(sizeof(node));	
			p->data = x;
			if(++i == 1)
				head->next = p;
			else
			   	q->next = p;
			
			q = p;
			
		}
	}
	q->next = NULL;
	return head;
}

int main()
{
	node *p = create();
	node *q = p;
	if (p->next == NULL)
	  printf(" %d", p->data);

	while(q)
	{
		printf("%d,", q->data);
		q = q->next;	
	}
}
