#include "Mutex.h"
#include <assert.h>
#include <unistd.h>


Mutex::Mutex():
	holder_(0)
{
	MCHECK(pthread_mutex_init(&mutex_, NULL));
}
Mutex::~Mutex()
{
	assert(holder_ == 0);
	pthread_mutex_destroy(&mutex_);
}
void Mutex::Lock()
{
	MCHECK(pthread_mutex_lock(&mutex_));
	AssianHolder();
}
void Mutex::Unlock()
{
	//assert(holder_==static_cast<pid_t>(::syscall(SYS_gettid)));
	assert(holder_==gettid());
	UnAssianHolder();
	MCHECK(pthread_mutex_unlock(&mutex_));
}
pthread_mutex_t* Mutex::GetMutex()
{
	return &mutex_;
}
void Mutex::AssianHolder()
{
	//holder_=static_cast<pid_t>(::syscall(SYS_gettid));		//can be optimized
	holder_=gettid();
}
void Mutex::UnAssianHolder()
{
	holder_=0;
}

MutexGuard::MutexGuard(Mutex& mutex)
	:mutex_(mutex)
{
	mutex_.Lock();
}
MutexGuard::~MutexGuard()
{
	mutex_.Unlock();
}