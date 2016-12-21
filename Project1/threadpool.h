
#ifndef threadpool_h__
#define threadpool_h__

#include <list>
#include <stdio.h>
#include <exception>
#include <pthread.h>

#include "MutexLock.h"

using namespace std;

template< typename T>
class ThreadPool
{
public:
	ThreadPool(int thread_num = 4, int max_requests = 10000);
	~ThreadPool();
	bool append(T* request);
	void start();
	void stop();

private:
	static void* worker(void* arg);
	void run();

private:
	int m_thread_num;
	int max_request_num;
	pthread_t* m_threads;
	list<T*> work_queue;
	MutexLock mutex_lock;
	sem m_queuestart;
	bool running;
};


#endif // threadpool_h__