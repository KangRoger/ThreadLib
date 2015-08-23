#include "../../src/BaseThread.h"
#include <iostream>
#include <string>
#include <unistd.h>

class Thread: public BaseThread
{
public:
	Thread(std::string& name);
	~Thread();
	void DoTask(void* argument);
};

Thread::Thread(std::string& name)
	:BaseThread(name)
{}
Thread::~Thread()
{}

void Thread::DoTask(void* argument)
{
	std::cout<<"Hello World"<<std::endl;
}

int main()
{
	std::string name="Thread";
	Thread t(name);
	t.Run();
	//t.Join();
	sleep(1);
	return 0;
}
