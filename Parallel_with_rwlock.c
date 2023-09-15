#include <pthread.h>
#include "operations.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "globals.h"
#include "LinkedList.h"


int division;


// perform_operations performs the operations in the op_array for a given thread
void* perform_operations_rwlock(void * rank) {

    // get the arguments
    int thread_rank = (int) rank;

    int start = thread_rank * division;
    int end = start + division;

    for (int j = start; j < end; j++) {
        int rand_value = rand() % (2 << 16);
        switch (op_array[j]) {
            case 0:
                pthread_rwlock_rdlock(&rwlock); // Acquire a read lock
                Member(rand_value, dup_head);
                pthread_rwlock_unlock(&rwlock); // Release the read lock
                break;
            case 1:
                pthread_rwlock_wrlock(&rwlock); // Acquire a write lock
                Insert(rand_value, &dup_head);
                pthread_rwlock_unlock(&rwlock); // Release the write lock
                break;
            case 2:
                pthread_rwlock_wrlock(&rwlock); // Acquire a write lock
                Delete(rand_value, &dup_head);
                pthread_rwlock_unlock(&rwlock); // Release the write lock
                break;
        }
    }
    return NULL;
}

double parallel_with_rwlock (int thread_count) {
    clock_t start_time, end_time;

    //    initialize_list(&head, n);
    division = m / thread_count;

    // create thread handlers array
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    // measure the time taken to perform the operations
    start_time = clock();

    // create the threads
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, perform_operations_rwlock, (void *) i);
    }

    // wait for the threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    end_time = clock();

    // print the time taken to perform the operations
//    printf("Time taken to perform the operations: %f ms\n", ((double)(end_time - start_time)) * 1000 / CLOCKS_PER_SEC);

    return ((double)(end_time - start_time)) * 1000 / CLOCKS_PER_SEC;
}
