
#ifndef Condition_h__
#define Condition_h__

#include <pthread.h>
#include <errno.h>

#include "MutexLock.h"

class Condition : noncopyable
{
public:
	Condition(MutexLock& lock)
		:mutex_(lock)
	{
		pthread_cond_init(&pcond_);
	}

	~Condition()
	{
		pthread_cond_destroy(&pcond_);
	}

	void notify()
	{
		MutexLockGuard lock(mutex_);
		pthread_cond_signal(&pcond_);
	}

	void notifyall()
	{
		MutexLockGuard lock(mutex_);
		pthread_cond_broadcast(&pcond_);
	}

	void wait()
	{
		pthread_cond_wait(&pcond_, mutex_.getPThreadMutex());
	}

	bool waitForSenconds(double seconds)
	{
		struct timespec abstime;
		// FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
		clock_gettime(CLOCK_REALTIME, &abstime);

		const int64_t kNanoSecondsPerSecond = 1e9;
		int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

		abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
		abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

		MutexLock::UnassignGuard ug(mutex_);
		return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
	}


private:
	pthread_cond_t pcond_;
	MutexLock mutex_;
};
#endif // Condition_h__