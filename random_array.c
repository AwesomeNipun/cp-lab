#include <stdio.h>
#include <malloc.h>
#include "globals.h"

// array to store the operations to be performed
int* createArray(int m, double member, double insert, double delete){

    // free the memory
    free(op_array);

    int members = (int) (member*m);
    int insertions = (int) (insert*m);
    int deletions = (int) (delete*m);

    int* newArray = (int*)malloc(m * sizeof(int));

    int i;
    for (i = 0; i < members; i++) {
        newArray[i] = 0;
    }

    for (i = members; i < (members+insertions); i++) {
        newArray[i] = 1;
    }

    for (i = (members+insertions); i < (members+insertions+deletions); i++) {
        newArray[i] = 2;
    }
    for (int k = m - 1; k > 0; k--) {
        int j = rand() % (k + 1); // Generate a random index between 0 and i

        // Swap arr[i] and arr[j]
        int temp = newArray[k];
        newArray[k] = newArray[j];
        newArray[j] = temp;
    }
    return newArray;
}