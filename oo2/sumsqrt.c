#include <pthread.h>
#include <stdio.h>
#include <math.h>

typedef struct runpars{
	int lower;
	int upper;
	float res;
} runpars;

void *runner(void *params);
void runThread(pthread_t *tidP, runpars *params);
void spawnNThreads(int n, runpars pars[]);

int main(int argc, char* argv[]){
	if(argc != 3){
		fprintf(stderr, "usage: sumsqrt [n] [threads]\n");
		return -1;
	}
	if(atoi(argv[1]) < 0) {
		fprintf(stderr,"%d - n must be >= 0\n", atoi(argv[1]));
		return -1;
	}
	if(atoi(argv[2]) <= 0) {
		fprintf(stderr,"%d - threads must be = 0\n", atoi(argv[2]));
		return -1;
	}

	int upper = atoi(argv[1]);
	int nThreads = atoi(argv[2]);
	int stepSize = upper/nThreads;
	struct runpars pars[nThreads];
	int i;
	for (i=0; i<nThreads; i++){
		int first = i * stepSize;
		int last = (i == nThreads-1) ? upper : (i+1) * stepSize -1;
		pars[i] = (runpars) {.lower = first, .upper = last, .res = 0.0};
		//printf("%d to %d\n", pars[i].lower, pars[i].upper);
	}
	spawnNThreads(nThreads, pars);
	float sum = 0.0;
	for (i=0; i<nThreads; i++){
		sum += pars[i].res;
	}
	printf("sum of squares 0 though %d = %f\n", upper, sum);

	return 0;
}

void spawnNThreads(int n, runpars pars[]){
	pthread_t ids[n];
	int i;
	for (i=0; i<n; i++){
		runThread(&(ids[i]), &(pars[i]));
	}
	for (i=0; i<n; i++)
		pthread_join(ids[i], NULL);
}

void runThread(pthread_t *tidP, runpars *params){
	pthread_attr_t attrP;
	pthread_attr_init(&attrP);
	pthread_create(tidP, &attrP, runner, params);
	pthread_attr_destroy(&attrP);
}

void *runner(void *params){
	struct runpars* pars = (struct runpars*) params;
	int i, upper = pars->upper;
	for(i = pars->lower; i <= upper; i++){
		pars->res += sqrtf(i);
	}
	pthread_exit(0);
}
