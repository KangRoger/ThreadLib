#ifndef BASETHREAD_H_
#define BASETHREAD_H_
#include <string>
#include <pthread.h>
class BaseThread
{
public:
	BaseThread(const std::string& name);
	virtual ~BaseThread();
	 void Run();
	void Join();
	static void* ThreadProxy(void* argument);
protected:
	virtual void DoTask() = 0;

private:
	void Initilization();
	bool initilized_;
	bool started_;
	bool joined_;
	pthread_t pthreadId_;
	pid_t tid_;
	std::string name_;

};
#endif	//BASETHREAD_H_