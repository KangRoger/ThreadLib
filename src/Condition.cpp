#include "Condition.h"
#include "Mutex.h"
#include <time.h>
#include <errno.h>

Condition::Condition(Mutex& mutex)
	:mutex_(mutex)
{
	MCHECK(pthread_cond_init(&cond_, NULL));
}
Condition::~Condition()
{
	MCHECK(pthread_cond_destroy(&cond_));
}
void Condition::Wait()
{
	mutex_.UnAssianHolder();
	MCHECK(pthread_cond_wait(&cond_, mutex_.GetMutex()));
	mutex_.AssianHolder();
}
bool Condition::TimeWait(int second)
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	now.tv_sec+=second;
	mutex_.UnAssianHolder();
	int result=pthread_cond_timedwait(&cond_, 
		mutex_.GetMutex(),
		&now);
	mutex_.AssianHolder();
	return result==ETIMEDOUT;
}
void Condition::Notify()
{
	MCHECK(pthread_cond_signal(&cond_));
}
void Condition::NotifyAll()
{
	MCHECK(pthread_cond_broadcast(&cond_));
}