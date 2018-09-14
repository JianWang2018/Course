#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *produce(void *);
void *consume(void *);

int tmp[5];
int main(int argc, char* argv[]){

     pthread_t thread1;
     pthread_t thread2;
     int i, j;
     int tt[5];

     for (j=0; j<5; j++) {
       tt[j] = j;
       if ((i=pthread_create(&thread1, NULL, produce, (void*)&(tt[j]))) != 0) {
	 printf("thread creation failed. %d\n", j);
       }
     }

     if ((i=pthread_create(&thread2, NULL, consume, (void*)NULL)) != 0) {
       printf("thread creation failed. %d\n", i);
     }

     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);

     printf("Exiting main\n");
     return 0;

}

void *produce(void *arg){
  int i;
  int me;
  me = *(int *)arg;
  for(i=0;i<10;i++){
    printf("I am %d, I produce item %d\n", me, i);
    sleep(1);
  }
  return NULL;
}

void *consume(void *arg){
  int i;
  for(i=0;i<10;i++){
    printf("I consume\n");
    sleep(1);
  }
  return NULL;
}


