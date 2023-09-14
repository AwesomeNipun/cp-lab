// this file is to implement the parallel implementation of the linked list operations with one mutex
#include <stdio.h>
#include <pthread.h>
#include "operations.h"
#include <stdlib.h>
#include <time.h>
#include "random_array.h"
#include "globals.h"

/*int n = 1000; // number of elements in the list
const int m = 10000; // number of operations
const double member_fraction = 0.99; // fraction of operations that are member operations
const double insert_fraction = 0.005; // fraction of operations that are insert operations
const double delete_fraction = 0.005; // fraction of operations that are delete operations

int thread_count = 1; // number of threads

int member_count = (int) (member_fraction * m);
int insert_count = (int) (insert_fraction * m);
int delete_count = (int) (delete_fraction * m);

*/
/*
struct list_node* head = NULL;
int* op_array;*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // initialize the mutex
int division_one_mutex;

/*
void initialize_list(struct list_node** head, int n);*/

// perform_operations performs the operations in the op_array for a given thread
void* perform_operations (void *rank) {
    int thread_rank = (int) rank;
    // start for the thread
    int start = thread_rank * division_one_mutex;
    // end for the thread
    int end = start + division_one_mutex;

    int* array = op_array;

    // divide the work among threads
    for (int j=start; j<end; j++){
        int rand_value = rand() % (2<<16);

        if (array[j] % 3 == 0) {
            pthread_mutex_lock(&mutex);
            Member(rand_value, head);
            pthread_mutex_unlock(&mutex);
        } else if (array[j] % 3 == 1) {
            pthread_mutex_lock(&mutex);
            Insert(rand_value, &head);
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_mutex_lock(&mutex);
            Delete(rand_value, &head);
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

double parallel_with_one_mutex(struct list_node* head, int thread_count) {
    clock_t start_time, end_time;

    // create the array of operations
//    op_array = createArray(m, member_fraction, insert_fraction, delete_fraction);

    // print array and count the number of each operation
    int i;
    int member_count = 0;
    int insert_count = 0;
    int delete_count = 0;

    for (i = 0; i < m; i++) {
        if (op_array[i] == 0) {
            member_count++;
        } else if (op_array[i] == 1) {
            insert_count++;
        } else {
            delete_count++;
        }
    }

    printf("Member count: %d\n", member_count);
    printf("Insert count: %d\n", insert_count);
    printf("Delete count: %d\n", delete_count);

//    initialize_list(&head, n);

    pthread_t threads[thread_count];

    division_one_mutex = m / thread_count;

    // measure the time taken to perform the operations
    start_time = clock();

    // create thread handlers array
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    // create the threads
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&thread_handles[i], NULL, perform_operations, (void*) i);
    }

    // wait for the threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    end_time = clock();
    printf("Time taken: %f\n", ((double) (end_time - start_time)) / CLOCKS_PER_SEC);

    return ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
}
/*
// initialize_list initializes the list with n random values
void initialize_list(struct list_node** head, int n) {

    int count = 0;
    while (true) {
        // generate a random number between 0 and 1000
        int rand_value = rand() % (2<<16);
        int value = Insert(rand_value, head);

        if (value == 0) {
            continue;
        }
        if (count == n) {
            break;
        }
        count++;
    }
}*/



