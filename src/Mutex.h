#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/syscall.h>
//#define gettid() syscall(__NR_gettid)
#define gettid() syscall(SYS_gettid)

#define MCHECK(result) ({ __typeof__ (result) errnum = (result);;	\
		assert(errnum == 0);	\
		(void) errnum;})




class Mutex
{
public:
	Mutex();
	~Mutex();
	void Lock();
	void Unlock();
	pthread_mutex_t* GetMutex();
	void AssianHolder();
	void UnAssianHolder();
private:
	pthread_mutex_t mutex_;
	pid_t holder_;
};

class MutexGuard
{
public:
	explicit MutexGuard(Mutex& mutex);
	~MutexGuard();
private:
	Mutex mutex_;
};

#endif //MUTEX_H_