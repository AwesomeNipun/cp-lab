#include <stdio.h>
#include <pthread.h>
#include "operations.h"
#include <stdlib.h>
#include <time.h>
#include "random_array.h"
#include "serial_impl.h"
#include "parallel_with_one_mutex.h"

double cases[3][3] = {
        {0.99, 0.005, 0.005},
        {0.9,  0.05,  0.05},
        {0.5,  0.25,  0.25}};

int thread_counts[4] = {1, 2, 4, 8};
int n = 1000;
int m = 10000;
struct list_node *head = NULL;

void initialize_list(struct list_node **head, int n);

struct list_node *copyList(struct list_node *head);

void destroyLinkedList(struct list_node *head);

int *op_array;

int main() {
    initialize_list(&head, n);
    for (int i = 0; i < 3; i++) {
        op_array = createArray(m, cases[i][0], cases[i][1], cases[i][2]);
        struct list_node *dup_head = copyList(head);
        double time = serial_impl(dup_head, n, m, op_array);
        destroyLinkedList(dup_head);
        dup_head = NULL;
        printf("Time taken in main serial: %f\n", time);

        for (int j = 0; j < 4; j++){
            /*dup_head = copyList(head);
            time = parallel_with_one_mutex(dup_head, n, m, op_array,thread_counts[j]);
            destroyLinkedList(dup_head);
            printf("Time taken in main parallel mutex: %f\n", time);
            dup_head = NULL;*/

            /*dup_head = copyList(head);
            time = serial_impl(dup_head, n, m, op_array);
            destroyLinkedList(dup_head);
            printf("Time taken in main parallel: %f\n", time);*/

        }
    }
}

void initialize_list(struct list_node **head, int n) {

    int count = 0;
    while (true) {
        // generate a random number between 0 and 1000
        int rand_value = rand() % (2 << 16);
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

struct list_node *copyList(struct list_node *head) {
    if (head == NULL) {
        return NULL; // Return NULL for an empty list
    }

    // Create a new node for the copied list
    struct list_node *newNode = (struct list_node *) malloc(sizeof(struct list_node));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }
    newNode->data = head->data;
    newNode->next = copyList(head->next); // Recursive call to copy the rest of the list

    return newNode;
}

void destroyLinkedList(struct list_node *head) {
    struct list_node *current = head;
    while (current != NULL) {
        struct list_node *temp = current;
        current = current->next;
        free(temp); // Free the current node
    }
    head = NULL; // Set the head pointer to NULL to indicate an empty list
}
