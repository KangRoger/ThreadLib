#include "../../src/Mutex.h"
#include "../../src/Condition.h"
#include <stdlib.h>  
#include <pthread.h> 
#include <stdio.h>  
#include <unistd.h>



struct msg{  
    struct msg *next;  
    int num;  
};  

struct msg *head;  

Mutex mutex;
Condition con(mutex);


void *consumer(void* argument)//消费者  
{  
    struct msg *mp;  
    for(;;)
    { 
        {
            MutexGuard lock(mutex);
            while(head==NULL)//无货的话，等待生产者生产  
                con.Wait();
            mp=head;  
            head=mp->next;   
        }

        printf("Consume %d\n",mp->num);  
        free(mp);  
        sleep(rand()%5);  
    }  
}  
void* producer(void* argument)//生产者  
{  
    struct msg *mp;  
    for(;;){  
        mp=static_cast<msg*> (malloc(sizeof(struct msg)));  
        mp->num=rand()%1000+1;  
        printf("Produce %d\n",mp->num);  
        {
            MutexGuard lock(mutex);
            mp->next=head;  
            head=mp;  
        }
        con.Notify();
        sleep(rand()%5);  
    }  
}  
int main(int argc, char **argv)  
{  

    pthread_t tidA,tidB;  
    pthread_create(&tidA, NULL, producer, NULL);  
    pthread_create(&tidB, NULL, consumer, NULL);  
  
    pthread_join(tidA,NULL);  
    pthread_join(tidB,NULL); 
    return 0;  
}
