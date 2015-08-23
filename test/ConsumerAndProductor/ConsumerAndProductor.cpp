#include "../../src/BaseThread.h"
#include "../../src/Mutex.h"
#include "../../src/Condition.h"
#include <stdlib.h>  
#include <stdio.h>  
#include <unistd.h>

struct msg{  
    struct msg *next;  
    int num;  
};  

struct msg *head;  

Mutex mutex;
Condition cond(mutex);

class Productor: public BaseThread
{
public:
	Productor(const std::string& name);
	~Productor();
	void DoTask();
};
Productor::Productor(const std::string& name)
	:BaseThread(name)
{}
Productor::~Productor()
{}
void Productor::DoTask()
{
 	struct msg *mp;  
    	for(;;)
    	{  
        		mp=static_cast<msg*> (malloc(sizeof(struct msg)));  
        		mp->num=rand()%1000+1;  
        		printf("Produce %d\n",mp->num);  
        		{
            			MutexGuard lock(mutex);
            			mp->next=head;  
            			head=mp;  
        		}
        			cond.Notify();
        		sleep(rand()%5);  
    	}  
}

class Consumer: public BaseThread
{
public:
	Consumer(const std::string& name);
	~Consumer();
	void DoTask();
};
Consumer::Consumer(const std::string& name)
	:BaseThread(name)
{}
Consumer::~Consumer()
{}
void Consumer::DoTask()
{
	struct msg *mp;  
    	for(;;)
    	{ 
        		{
            			MutexGuard lock(mutex);
            			while(head==NULL)//无货的话，等待生产者生产  
                			cond.Wait();
            			mp=head;  
            			head=mp->next;   
        		}

        		printf("Consume %d\n",mp->num);  
        		free(mp);  
        		sleep(rand()%5);  
    	}  

}

int main()
{
	Consumer consumer("Consumer");
	Productor productor("Productor");
	consumer.Run();
	productor.Run();

	consumer.Join();
	productor.Join();
	return 0;
}