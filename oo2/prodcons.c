#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "list/list.h"

typedef struct runpars{
	int start;
	int end;
} runpars;

void *producer(void *params);
void *consumer(void *params);
void runThread(pthread_t *tidP, runpars *params);
pthread_t* spawnNThreads(int n, void*(*fun) (void *));
void sleeper(float waitMs);
int prodN;
int consN;
int bufferSize;
int productsN;
int producerLoad;
int consumerLoad;
float SLEEP_TIME_MS = 1.0;
List *buffer;
sem_t empty, full;

int main(int argc, char* argv[]){
	setbuf(stdout,NULL);
	if(argc != 5){
		fprintf(stderr, "usage: prodcons [procucers] [consumers] [bufferSize] [products]\n");
		return -1;
	}
	if(atoi(argv[1]) <= 0) {
		fprintf(stderr,"%d - producers must be > 0\n", atoi(argv[1]));
		return -1;
	}
	if(atoi(argv[2]) <= 0) {
		fprintf(stderr,"%d - consumers must be > 0\n", atoi(argv[2]));
		return -1;
	}
	if(atoi(argv[3]) <= 0) {
		fprintf(stderr,"%d - bufferSize must be > 0\n", atoi(argv[3]));
		return -1;
	}
	if(atoi(argv[4]) <= 0) {
		fprintf(stderr,"%d - products must be > 0\n", atoi(argv[4]));
		return -1;
	}

	buffer = list_new();
	prodN = atoi(argv[1]);
	consN = atoi(argv[2]);
	bufferSize = atoi(argv[3]);
	productsN = atoi(argv[4]);
	producerLoad = productsN / prodN;
	consumerLoad = productsN / consN;
	printf("Starting! Here are the stats\n"
			"prodN: %i\n"
			"consN: %i\n"
			"buffer: %i\n"
			"products: %i\n"
			"produderload: %i\n"
			"consumerload: %i\n",
			prodN, consN, bufferSize, productsN, producerLoad, consumerLoad);

	sem_init(&full, 0, 0);
	sem_init(&empty, 0, bufferSize);
	srand(time(NULL));

	pthread_t *consIds = spawnNThreads(consN, consumer);
	pthread_t *prodIds = spawnNThreads(prodN, producer);
	int i;
	for(i=0; i<prodN; i++){
		pthread_join(prodIds[i], NULL);
	}
	for(i=0; i<consN; i++){
		pthread_join(consIds[i], NULL);
	}
	free(prodIds);
	free(consIds);
	return 0;
}

pthread_t* spawnNThreads(int n, void *(*fun)(void *)){
	pthread_t *ids = (pthread_t*) malloc(sizeof(pthread_t)*n);
	int i;
	for (i=0; i<n; i++){
		struct runpars *par = malloc(sizeof(runpars));
		if(fun==producer){
		par->start = producerLoad*i;
		par->end = i==n-1 ? productsN -1 : (producerLoad * (i+1))-1;
		printf("produce steps: %i - %i\n", par->start, par->end);
		}else{
		par->start = consumerLoad*i;
		par->end = i==n-1 ? productsN -1 : (consumerLoad * (i+1))-1;
		printf("consume steps: %i - %i\n", par->start, par->end);
		}
		pthread_create(&ids[i], NULL, fun, par);
	}
	return ids;
}

void *producer(void *params){
	struct runpars* pars = (struct runpars*) params;
	int i;
	for(i = pars->start; i < pars->end; i++){
		Node *n = node_new();
		n->elm = (void*) i;
		sem_wait(&empty);
		list_add(buffer, n);
		sem_post(&full);
		sleeper(SLEEP_TIME_MS);
		int fullC,emptyC;
		sem_getvalue(&full,&fullC);
		sem_getvalue(&empty,&emptyC);
		printf("producer %i made item %i, full: %i, empty: %i\n", pars->start/producerLoad, (int) n->elm, fullC, emptyC);
	}
	free(pars);
	pthread_exit(0);
}

void *consumer(void *params){
	struct runpars* pars = (struct runpars*) params;
	int i;
	for(i = pars->start; i < pars->end; i++){
		sem_wait(&full);
		Node *n = list_remove(buffer);
		sem_post(&empty);
		sleeper(SLEEP_TIME_MS);
		int fullC,emptyC;
		sem_getvalue(&full,&fullC);
		sem_getvalue(&empty,&emptyC);
		printf("consumer ate item %i, full: %i, empty: %i\n", (int) n->elm, fullC, emptyC);
	}
	free(pars);
	pthread_exit(0);
}

void sleeper(float waitMs){
	//random value between 0 and waitMs
	waitMs = (float)rand() * waitMs / (float)RAND_MAX;
	usleep((int)waitMs * 1000); //times 1000 for microseconds;
}
