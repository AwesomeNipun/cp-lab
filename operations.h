#ifndef SOURCE_CODE_OPERATIONS_H
#define SOURCE_CODE_OPERATIONS_H

struct list_node { // define the structure of the linked list
    int data;
    struct list_node *next;
};

int Member(int value, struct list_node* head);
int Insert(int value, struct list_node** head);
int Delete(int value, struct list_node** head);
void traverse_list(struct list_node* head);

#endif //SOURCE_CODE_OPERATIONS_H
