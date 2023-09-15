// this file is to implement the serial implementation of the linked list operations
#include <stdio.h>
#include "operations.h"
#include <stdlib.h>
#include <time.h>
#include "random_array.h"
#include "globals.h"
#include "LinkedList.h"

double serial_impl() {
    clock_t start_time, end_time;

    // measure the time taken to perform the operations
    start_time = clock();
    int MAX_INT = 2 << 16;

    for (int i = 0; i < m; i++) {
        int rand_value = rand() % MAX_INT;
        switch (op_array[i]) {
            case 0:
                Member(rand_value, dup_head);
                break;
            case 1:
                Insert(rand_value, &dup_head);
                break;
            case 2:
                Delete(rand_value, &dup_head);
                break;
        }
    }
    end_time = clock();



//    printf("Serial implesmentation took %f ms\n", ((double)(end_time - start_time)) * 1000 / CLOCKS_PER_SEC);
    return ((double)(end_time - start_time)) * 1000 / CLOCKS_PER_SEC;
}
