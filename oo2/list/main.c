/******************************************************************************
	 main.c

	 Implementation of a simple FIFO buffer as a linked list defined in list.h.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"
#include "assert.h"

// FIFO list;
List *fifo;
const int elmsPerThread = 500;
const int numberOfThreads = 3;

void *asyncRunner(void* peh){
	int i;
	for(i=0; i<elmsPerThread; i++){
		list_add(fifo, node_new());
	}
	pthread_exit(NULL);
}
void asyncListTest(){ //this test is fake and gay, and not unit-like at all
	fifo = list_new();
	pthread_t *ids[numberOfThreads];
	int i;
	for(i=0; i<numberOfThreads; i++){
		pthread_t* tid = (pthread_t *) malloc(sizeof(pthread_t));
		ids[i] = tid;
		pthread_create(tid, NULL, asyncRunner, NULL);
	}
	for(i=0; i<numberOfThreads; i++){
		pthread_join(*ids[i], NULL);
	}
	int removeN=0;
	int actualN=0;
	int assertedN = fifo->len;
	Node *n = fifo->first->next;
	while(n != NULL) {
		actualN++;
		n = n->next;
	}

	n = list_remove(fifo);
	while(n != NULL){
		removeN++;
		n = list_remove(fifo);
	}
	//printf("asserted: %i\nremoved: %i\nactual: %i\nelmsPer: %i\n#of threads: %i\nprod: %i\n", assertedN, removeN, actualN, elmsPerThread, numberOfThreads, elmsPerThread*numberOfThreads);
	assert(assertedN == actualN);                       //fifo->len is the same as the actual length
	assert(removeN == actualN);                         //remove will yield as many elms as there actually are in the list
	assert(elmsPerThread*numberOfThreads == actualN);   //there are actually the expected number of elements
	assert(fifo->len == 0);                             //the list is now empty
}

int main(int argc, char* argv[])
{
	asyncListTest();
	return 0;
	fifo = list_new();

	list_add(fifo, node_new_str("s1"));
	list_add(fifo, node_new_str("s2"));

	Node *n1 = list_remove(fifo);
	if (n1 == NULL) { printf("Error no elements in list\n"); exit(-1);}
	Node *n2 = list_remove(fifo);
	if (n2 == NULL) { printf("Error no elements in list\n"); exit(-1);}
	printf("%s\n%s\n", n1->elm, n2->elm);

	return 0;
}

