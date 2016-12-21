
#ifndef MutexLock_h__
#define MutexLock_h__

#include <pthread.h>

#include "nocopyable.h"

class MutexLock: noncopyable
{
public:
	MutexLock()
	{
		pthread_mutex_init(&mutex_, NULL);
	}
	~MutexLock()
	{
		pthread_mutex_destroy(&mutex_);
	}

	void Lock()
	{
		pthread_mutex_lock(&mutex_);
	}

	void Unlock()
	{
		pthread_mutex_unlock(&mutex_);
	}

	pthread_mutex_t* getPThreadMutex()
	{
		return &mutex_;
	}

private:
	pthread_mutex_t mutex_;
};

class MutexLockGuard
{
public:
	explicit MutexLockGuard(MutexLock& mutex)
		:mutexLock_(mutex)
	{
		mutexLock_.Lock();
	}
	~MutexLockGuard()
	{
		mutexLock_.Unlock();
	}

private:
	MutexLock mutexLock_;
};

#endif // MutexLock_h__