#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include "operations.h"

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

    printf("List initialized with %d nodes\n", count);
}

struct list_node *copyList(struct list_node *head) {

    int count = 0;

    struct list_node *new_head = NULL;
    struct list_node *current = head;

    while (current != NULL) {
        count++;
        Insert(current->data, &new_head);
        current = current->next;
    }

    return new_head;
}

void destroyLinkedList(struct list_node *head) {
    struct list_node *current = head;
    struct list_node *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp); // Free the current node
    }
}
