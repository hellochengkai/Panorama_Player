#include "StdAfx.h"
#include "MyQueue.h"


MyQueue::MyQueue(void)
{
	Queuelock.Lock();
	Count = 0;
	listHead = NULL;
	Queuelock.Unlock();
}


MyQueue::~MyQueue(void)
{
	Queuelock.Lock();
	Count = 0;
	listHead = NULL;
	Queuelock.Unlock();
}

int MyQueue::PushQueue(void * p)
{
	int Ret = 0;

	Mylist * newNode = (Mylist *)malloc(sizeof(Mylist));
	if(!newNode){
		Ret = -1;
		goto EXIT;
	}
	Queuelock.Lock();
	memset(newNode,0,sizeof(Mylist));
	newNode->next = listHead;
	newNode->p = p;
	listHead = newNode;
	Count++;
	Queuelock.Unlock();
EXIT:
	return Ret;
}

int MyQueue::GetQueue(void ** p)
{

	int Ret = 0;
	Queuelock.Lock();	
	Mylist * cur_p = listHead;


	if(cur_p == NULL){
		*p = NULL;
	}else if(cur_p->next == NULL){
		*p = cur_p->p;
		free(cur_p);
		Count--;
		cur_p = NULL;
		listHead = NULL;
	}else{
		while(cur_p->next){
			if(cur_p->next->next == NULL)
				break;
			cur_p = cur_p->next;
		}
		*p = cur_p->next->p;
		free(cur_p->next);
		Count--;
		cur_p->next = NULL;
	}
	Queuelock.Unlock();
	return Ret;
}
int MyQueue::printf_Queue()
{
	int Ret = 0;

	Queuelock.Lock();
	Mylist * cur_p = listHead;
	while(cur_p){
		printf("Count %d   cur_p === %p \n",Count,cur_p);
		cur_p = cur_p->next;
	}
	Queuelock.Unlock();
	return Ret;
}
