#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "list/list.h"

typedef struct runpars{
	int offset;
} runpars;

void *producer(void *params);
void *consumer(void *params);
void runThread(pthread_t *tidP, runpars *params);
pthread_t* spawnNThreads(int n, void*(*fun) (void *));
int prodN;
int consN;
int bufferSize;
int productsN;
int producerLoad;
List *buffer;
sem_t empty, full;

int main(int argc, char* argv[]){
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
	producerLoad = prodN / productsN;

	sem_init(&full, 0, bufferSize);
	sem_init(&empty, 0, bufferSize);

	pthread_t *ids = spawnNThreads(prodN, producer);
	int i;
	for(i=0; i<prodN; i++){
		pthread_join(ids[i], NULL);
	}
	free(ids);
	//int upper = atoi(argv[1]);
	//int nThreads = atoi(argv[2]);
	//int stepSize = upper/nThreads;
	//struct runpars pars[nThreads];
	//int i;
	//for (i=0; i<nThreads; i++){
	//	int first = i * stepSize;
	//	int last = (i == nThreads-1) ? upper : (i+1) * stepSize -1;
	//	pars[i] = (runpars) {.lower = first, .upper = last, .res = 0.0};
	//	//printf("%d to %d\n", pars[i].lower, pars[i].upper);
	//}
	//spawnNThreads(nThreads, pars);
	//float sum = 0.0; //for (i=0; i<nThreads; i++){
	//	sum += pars[i].res;
	//}
	//printf("sum of squares 0 though %d = %f\n", upper, sum);

	return 0;
}

pthread_t* spawnNThreads(int n, void*(*fun) (void *)){
	pthread_t *ids = (pthread_t*) malloc(sizeof(pthread_t)*n);
	int i;
	for (i=0; i<n; i++){
		struct runpars *par = malloc(sizeof(runpars));
		par->offset = 1;
		pthread_create(&ids[i], NULL, fun, par);
	}
	return ids;
}

void *producer(void *params){
	struct runpars* pars = (struct runpars*) params;
	int i;
	for(i = 0; i < pars->offset; i++){
		Node *n = node_new();
		n->elm = (void*) i;
		list_add(buffer, n);
		printf("producer made item %i\n", (int) n->elm);
	}
	free(pars);
	pthread_exit(0);
}

void *consumer(void *params){
	struct runpars* pars = (struct runpars*) params;
	int i;
	for(i = 0; i < pars->offset; i++){
		Node *n = list_remove(buffer);
		printf("consumer ate item %i\n", (int) n->elm);
	}
	free(pars);
	pthread_exit(0);
}
