#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int data_avail = 0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_cond = PTHREAD_COND_INITIALIZER;

void *produce(void *);
void *consume(void *);

int main(int argc, char* argv[]){

	pthread_t thread1;
	pthread_t thread2;

	if (pthread_create(&thread1, NULL, produce, (void*)NULL) != 0) {
		printf("thread creation failed\n");
	}

	if (pthread_create(&thread2, NULL, consume, (void*)NULL) != 0) {
		printf("thread creation failed\n");
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("Exiting main, ... \n");
	return 0;

}

void *produce(void *arg){
	int i;

	for(i=0;i<10;i++){
		pthread_mutex_lock(&mutex);
		printf("produce: data_avail = %d\n", ++data_avail);
		pthread_cond_signal(&data_cond);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void *consume(void *arg){
	int i = 10;

	for (;;) {
		pthread_mutex_lock(&mutex);
		while (! data_avail) {
			pthread_cond_wait(&data_cond, &mutex);
		}
		printf("consume: data_avail = %d\n", --data_avail);
		pthread_mutex_unlock(&mutex);
		--i;
		if (i == 0) 
			break;
	}
	return NULL;
}


