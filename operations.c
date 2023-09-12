#include <stdio.h>
#include <malloc.h>

// list_node is a struct that contains an integer and a pointer to the next node
struct list_node {
    int data;
    struct list_node *next;
};

// Member returns 1 if value is in the linked list, 0 otherwise
int Member (int value, struct list_node* head) {
    struct list_node* curr = head;

    while (curr != NULL && curr->data < value)
        curr = curr->next;

    if (curr == NULL || curr->data > value) // if the value is not in the list
        return 0;
    else
        return 1;
}

// Insert returns 1 if value was inserted into the linked list, 0 otherwise
int Insert (int value, struct list_node** head) {
    struct list_node* curr = *head;
    struct list_node* pred = NULL;
    struct list_node* temp;

    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr == NULL || curr->data > value) {
        temp = malloc(sizeof(struct list_node)); // allocate memory for the new node
        temp->data = value;
        temp->next = curr;
        if (pred == NULL) // if the value is the first element in the list
            *head = temp;
        else
            pred->next = temp; // insert the new node between pred and curr
        return 1;
    } else {
        return 0;
    }
}

// Delete returns 1 if value was deleted from the linked list, 0 otherwise
int Delete (int value, struct list_node** head) {
    struct list_node* curr = *head;
    struct list_node* pred = NULL;

    while (curr != NULL && curr->data < value) {
        pred = curr;
        curr = curr->next;
    }

    if (curr != NULL && curr->data == value){

        if (pred == NULL) { // if the value is the first element in the list
            *head = curr->next;
            free(curr); // free the memory
        } else {
            pred->next = curr->next;
            free(curr);
        }
        return 1;
    } else {
        return 0;
    }
}
