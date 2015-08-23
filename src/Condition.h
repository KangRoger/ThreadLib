#ifndef CONDITION_H_
#define CONDITION_H_
#include "Mutex.h"
class Condition
{
public:
	explicit Condition(Mutex& mutex);
	~Condition();
	void Wait();
	bool TimeWait(int second);
	void Notify();
	void NotifyAll();
private:
	Mutex mutex_;
	pthread_cond_t cond_;
};
#endif	//CONDITION_H_