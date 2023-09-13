// this file is to implement the serial implementation of the linked list operations
#include <stdio.h>
#include "operations.h"
#include <stdlib.h>
#include <time.h>
#include "random_array.h"

/*int n = 1000; // number of elements in the list
const int m = 10000; // number of operations
const double member_fraction = 0.99; // fraction of operations that are member operations
const double insert_fraction = 0.005; // fraction of operations that are insert operations
const double delete_fraction = 0.005; // fraction of operations that are delete operations

int member_count = (int) (member_fraction * m);
int insert_count = (int) (insert_fraction * m);
int delete_count = (int) (delete_fraction * m);

void initialize_list(struct list_node** head, int n);*/

double serial_impl(struct list_node* head, int n, int m, int* op_array) {
    clock_t start_time, end_time;

    // create the array of operations
//    int* op_array = createArray(m, member_fraction, insert_fraction, delete_fraction);

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

/*
    struct list_node* head = NULL;
    initialize_list(&head, n);
*/

    // measure the time taken to perform the operations
    start_time = clock();

    for (int j=0; j<n; j++) {
        int rand_value = rand() % (2 << 16);
        if (op_array[j] % 3 == 0) {
            Member(rand_value, head);
        } else if (op_array[j] % 3 == 1) {
            Insert(rand_value, &head);
        } else {
            Delete(rand_value, &head);
        }
    }
    end_time = clock();
    printf("Time taken: %f\n", ((double) (end_time - start_time)) / CLOCKS_PER_SEC);
//    traverse_list(head);
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
}
*/


