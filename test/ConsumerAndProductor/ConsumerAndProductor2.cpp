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

class Productor: public BaseThread
{
public:
	Productor(const std::string& name, Mutex& mutex, Condition& cond);
	~Productor();
	void DoTask();
private:
             Mutex& mutex_;
             Condition& cond_;
};
Productor::Productor(const std::string& name,
                Mutex& mutex,
                Condition& cond)
	:BaseThread(name),
               mutex_(mutex),
               cond_(cond)
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
            			MutexGuard lock(mutex_);
            			mp->next=head;  
            			head=mp;  
        		}
        		cond_.Notify();
        		sleep(rand()%5);  
    	}  
}

class Consumer: public BaseThread
{
public:
	Consumer(const std::string& name, Mutex& mutex, Condition& cond);
	~Consumer();
	void DoTask();
private:
            Mutex& mutex_;
            Condition& cond_;
};
Consumer::Consumer(const std::string& name, Mutex& mutex, Condition& cond)
	:BaseThread(name),
                mutex_(mutex),
                cond_(cond)
{}
Consumer::~Consumer()
{}
void Consumer::DoTask()
{
	struct msg *mp;  
    	for(;;)
    	{ 
        		{
            			MutexGuard lock(mutex_);
            			while(head==NULL)//无货的话，等待生产者生产  
                			cond_.Wait();
            			mp=head;  
            			head=mp->next;   
        		}

        		printf("Consume %d\n",mp->num);  
        		free(mp);  
        		sleep(rand()%5);  
    	}  

}

class ConsumerAndProductor
{
public:
    ConsumerAndProductor();
    ~ConsumerAndProductor();
    void Inilitization();
    void Run();
    void Join();
private:
    Mutex mutex_;
    Condition cond_;
    Consumer* consumer_;
    Productor* productor_;
};
ConsumerAndProductor::ConsumerAndProductor()
    :mutex_(Mutex()),
    cond_(Condition(mutex_)),
    consumer_(NULL),
    productor_(NULL)    
{}
ConsumerAndProductor::~ConsumerAndProductor()
{/*
    if(productor_)
        delete productor_;
    if(consumer_)
        delete consumer_;
        */
}
void ConsumerAndProductor::Inilitization()
{
    consumer_=new Consumer("Consumer", mutex_, cond_);
    productor_=new Productor("Productor", mutex_, cond_);
}
void ConsumerAndProductor::Run()
{
    consumer_->Run();
    productor_->Run();
}
void ConsumerAndProductor::Join()
{
    consumer_->Join();
    productor_->Join();
}


int main()
{
	ConsumerAndProductor consumerAndProductor;
              consumerAndProductor.Inilitization();
              consumerAndProductor.Run();
              sleep(10);
              consumerAndProductor.Join();
	return 0;
}