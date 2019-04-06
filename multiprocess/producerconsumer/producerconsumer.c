#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <process.h>

#define N 100
int count = 0;
char items[]={'a','b','c','d','e','f','g','h','i','j'};
int nItem=0;
#define FALSE 0
#define TRUE 1
void producer(void);
void consumer(void);
typedef struct item_list
{
  int top;
  int items[N];
}ITEM_LIST;

struct item_list item_list_instance;
int item_list_index=0;
int produce_item()
{
 nItem+=1;
 nItem%=10;
 return (int) items[nItem]; 
}
void insert_item(int item)
{
    if(item_list_instance.top < N)
    {
      item_list_instance.items[item_list_instance.top]=item;
      item_list_instance.top+=1;
    }
}
void consume_item(int item)
{
  
}
int remove_item()
{
   if(item_list_instance.top > 0)
   {
      item_list_instance.top-=1;
      return (int) item_list_instance.items[item_list_instance.top+1];
   }
   return -1;
}
void producer(void)
{
  int item;
  while(TRUE)
  {
    item  = produce_item();
    if(count == N) usleep(100);
    insert_item(item);
    count = count + 1;
    if(count == 1) wakeup(consumer);
    
  }
  
}

void consumer(void)
{
  int item;
  while(TRUE)
  {
    if(count == 0) usleep(100);
    item = remove_item();
    count = count - 1;
    if(count == N) wakeup(producer);
    consume_item(item);
  }
  
}

int main (int argc, char *argv[], char *arge[])
{
    pthread_t consumer_id, producer_id;
    item_list_instance.top = 0;
    
    pthread_create(&producer_id,NULL,producer,NULL);
    pthread_create(&consumer_id,NULL,consumer,NULL);
    while(TRUE)
    {
      
      sleep(1);
    }
}
