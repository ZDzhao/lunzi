#include "threadpool.h"


template< typename T>
ThreadPool<T>::ThreadPool(int thread_num /*= 4*/, int max_requests /*= 10000*/)
	:m_thread_num(thread_num), 
	max_request_num(max_request_num), 
	running(false),
	m_threads(NULL)
{
	if (m_thread_num < =0 || max_request_num <=0)
	{
		throw exception();
	}
}

template< typename T>
ThreadPool<T>::~ThreadPool()
{
	if (running)
	{
		stop();
	}
}

template< typename T>
void ThreadPool<T>::stop()
{
	running = false;
	delete[]m_threads;
}

template< typename T>
void ThreadPool<T>::start()
{	
	m_threads = new pthread_t[m_thread_num];
	if (!m_threads)
	{
		throw exception();
	}

	for (int i = 0; i <= m_thread_num; ++i)
	{
		if (pthread_create(m_threads[i], NULL, worker, this) != 0)
		{
			delete[]m_threads;
			throw exception();
		}

		if (pthread_detach(m_threads[i]))
		{
			delete[]m_threads;
			throw exception();
		}
	}
	running = true;
}

template< typename T>
void ThreadPool<T>::run()
{
	while (running)
	{
		T* request = NULL;
		m_queuestart.wait();
		{
			MutexLockGuard lock(mutex_lock);
			if (work_queue.empty())
			{
				continue;
			}
			request = work_queue.front();
			work_queue.pop_front();
		}
		if (!request)
		{
			request->process();
		}
	}
}

template< typename T>
void* ThreadPool<T>::worker(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	pool->run();
	return pool;
}

template< typename T>
bool ThreadPool<T>::append(T* request)
{
	{
		MutexLockGuard lock(mutex_lock);
		if (work_queue.size() >= max_request_num)
		{
			return false;
		}
		work_queue.push_back(request);
	}
	m_queuestart.post();
	return true;
}