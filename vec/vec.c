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

int length(node *head)
{

	int num = 1;
	node *q = head->next;
	while(q != NULL)
	{
		num++;	
		q = q->next;
	}
	return num;
}

bool excp(node* head, int pos)
{
	
	if (pos < 0)  printf("not correct num.\n");

	if (head == NULL)
	{
		printf("not enough link.\n");	
		return false;
	}
	return true;
}

node * search_node(node *head, int pos);

//
node *insert(node *head, int pos, int data)
{
	node *p = head, *q;
	
	node *item = NULL;
	item = (node *)malloc(sizeof(node));
	item->data = data;

	if (pos == 0)
	{
	     head->next = item;
	}

	q = search_node(head, pos);
	
	item->next = q->next;
	q->next = item;

	return p;
}


node *delete_node(node *head, int pos)
{
	node *item, *p = head;

	if (head->next == NULL)
	    return NULL;
	
	p = search_node(p, pos-1);
	if (p != NULL && p->next != NULL)
	{
		item = p->next;
		p->next = item->next;
		free(item);
	}
	return head;
}

node * search_node(node *head, int pos)
{
	node *p = head;
	if (pos < 0)  printf("not correct num.\n");

	if (p == NULL)
	{
		printf("not enough link.\n");	
		return NULL;
	}

	while(pos--)
	{
		if ((p = p->next) == NULL)
		break;
	}

	return p;
}

int main()
{
	node *p = create(), *item;
	node *q = p;

	if (p->next == NULL)
	  printf(" %d", p->data);

	while(q)
	{
		printf("%d,", q->data);
		q = q->next;	
	}
	printf("\n");
	
	q = insert(p, 6, 666);
	while(q)
	{
		printf("%d,", q->data);
		q = q->next;	
	}
	printf("\n");

	q = delete_node(p, 4);
	while(q)
	{
		printf("%d,", q->data);
		q = q->next;	
	}
	printf("\n");



	printf("length is :%d\n", length(p));

	printf("2nd data is :%d", search_node(p, 3)->data);
}









