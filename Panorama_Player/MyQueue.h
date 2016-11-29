#pragma once

typedef struct Mylist{
	void *p;
	Mylist * next;
}Mylist;

class MyQueue
{
public:
	MyQueue(void);
	~MyQueue(void);
	int PushQueue(void * p);
	int GetQueue(void ** p);
	int CountQueue(int *count){ 
		Queuelock.Lock();
		*count = Count;
		Queuelock.Unlock();
		return 0;
	};
	int printf_Queue();

private:
	Mylist * listHead;
	int Count;
	CCriticalSection Queuelock;
};

