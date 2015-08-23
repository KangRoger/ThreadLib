#include "BaseThread.h"
#include <unistd.h>
#include <assert.h>
#include <sys/syscall.h>

BaseThread::BaseThread(std::string& name)
	:initilized_(false),
	started_(false),
	joined_(false),
	pthreadId_(0),
	tid_(pid_t(0)),
	name_(name)
{}
BaseThread::~BaseThread()
{
	if(initilized_ && started_ && !joined_)
		pthread_detach(pthreadId_);
}
void BaseThread::Run()
{
	assert(!started_);
	started_=true;
	//create the thread
	pthread_create(&pthreadId_,
		NULL,
		BaseThread::ThreadProxy,
		this);
	//Initlilize the variables aobut thread

}
void BaseThread::Join()
{
	assert(started_);
	assert(!joined_);
	pthread_join(pthreadId_, NULL);
}

/*staitc function used to call the real Task
*/
void* BaseThread::ThreadProxy(void* argument) 
{
	BaseThread* thread=static_cast<BaseThread*>(argument);
	thread->Initilization();
	thread->started_=true;
	thread->DoTask(NULL);
	return static_cast<void*> (0);

}

void BaseThread::Initilization() //must be called by the new thread
{
	tid_=static_cast<pid_t>(::syscall(SYS_gettid));
	initilized_=true;
}