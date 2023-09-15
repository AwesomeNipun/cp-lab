// this file is to implement the parallel implementation of the linked list operations with one mutex
#include <pthread.h>
#include "operations.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "globals.h"
#include "LinkedList.h"


int division_one_mutex;


// perform_operations performs the operations in the op_array for a given thread
void* perform_operations (void * rank) {

    // get the arguments
    int thread_rank = (int) rank;
    // start for the thread
    int start = thread_rank * division_one_mutex;
    // end for the thread
    int end = start + division_one_mutex;

    // divide the work among threads
    for (int j=start; j<end; j++){
        int rand_value = rand() % (2<<16);

        switch (op_array[j]) {
            case 0:
                pthread_mutex_lock(&mutex);
                Member(rand_value, dup_head);
                pthread_mutex_unlock(&mutex);
                break;
            case 1:
                pthread_mutex_lock(&mutex);
                Insert(rand_value, &dup_head);
                pthread_mutex_unlock(&mutex);
                break;
            case 2:
                pthread_mutex_lock(&mutex);
                Delete(rand_value, &dup_head);
                pthread_mutex_unlock(&mutex);
                break;
        }
    }
    return NULL;
}

double parallel_with_one_mutex(int thread_count) {
    clock_t start_time, end_time;

    division_one_mutex = m / thread_count;

    // create thread handlers array
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    // measure the time taken to perform the operations
    start_time = clock();

    // create the threads
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, perform_operations, (void *) i);
    }

    // wait for the threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    end_time = clock();



//    printf("Parallel with one mutex: %f\n", ((double)(end_time - start_time)) * 1000 / CLOCKS_PER_SEC);
    return ((double)(end_time - start_time)) * 1000 / CLOCKS_PER_SEC;
}
