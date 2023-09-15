#include <stdio.h>
#include <pthread.h>
#include "operations.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "random_array.h"
#include "serial_impl.h"
#include "parallel_with_one_mutex.h"
#include "globals.h"
#include "Parallel_with_rwlock.h"

double cases[3][3] = {
        {0.99, 0.005, 0.005},
        {0.9,  0.05,  0.05},
        {0.5,  0.25,  0.25}};

int thread_counts[4] = {1, 2, 4, 8};
int n = 1000;
int m = 10000;
int ROUNDS = 100;
struct list_node *head = NULL;
int *op_array;

pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void initialize_list(struct list_node **head, int n);

struct list_node *copyList(struct list_node *head);

void destroyLinkedList(struct list_node *head);

double mean(const double arr[], int size);

double standard_deviation(const double arr[], int size, double mean_value);

void print_results(
        double serial_results[3][ROUNDS],
        double parallel_one_mutex_results[3][4][ROUNDS],
        double parallel_one_rwlock_results[3][4][ROUNDS]
);

void write_to_csv(
        double serial_results[3][ROUNDS],
        double parallel_one_mutex_results[3][4][ROUNDS],
        double parallel_rwlock_results[3][4][ROUNDS]
);

int main() {

    double serial_results[3][ROUNDS];
    double parallel_one_mutex_results[3][4][ROUNDS];
    double parallel_rwlock_results[3][4][ROUNDS];

    pthread_mutex_init(&mutex, NULL); // Initialize the mutex
    pthread_rwlock_init(&rwlock, NULL); // Initialize the read-write lock

    initialize_list(&head, n);
    for (int i = 0; i < 3; i++) {
        printf("\nCase %d\n", i);
        for (int r = 0; r< ROUNDS ; r++){
//            printf("Round %d\n", r);
            op_array = createArray(m, cases[i][0], cases[i][1], cases[i][2]);
            struct list_node *dup_head = copyList(head);
            serial_results[i][r] = serial_impl(dup_head);
            destroyLinkedList(dup_head);
            dup_head = NULL;
//            printf("Time taken in main serial: %f\n", serial_results[i][r]);

            for (int j = 0; j < 4; j++){
                dup_head = copyList(head);
                parallel_one_mutex_results[i][j][r] = parallel_with_one_mutex(dup_head ,thread_counts[j]);
                destroyLinkedList(dup_head);
//                printf("Time taken in main parallel mutex with %d threads: %f\n", thread_counts[j], parallel_one_mutex_results[i][j][r]);
                dup_head = NULL;

                dup_head = copyList(head);
                parallel_rwlock_results[i][j][r] = parallel_with_rwlock(dup_head, n, m, op_array,thread_counts[j]);
                destroyLinkedList(dup_head);
//                printf("Time taken in main parallel  with %d threads: %f\n",thread_counts[j], parallel_rwlock_results[i][j][r]);

            }
        }
    }
    print_results(serial_results, parallel_one_mutex_results, parallel_rwlock_results);
//    write_to_csv(serial_results, parallel_one_mutex_results, parallel_rwlock_results);
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
    struct list_node *new_head = NULL; // Initialize the new list's head to NULL
    struct list_node *tail = NULL; // Initialize a tail pointer to keep track of the last node

    struct list_node *current = head;

    while (current != NULL) {
        struct list_node *new_node = (struct list_node *)malloc(sizeof(struct list_node));
        if (new_node == NULL) {
            perror("Memory allocation failed");
            exit(1);
        }
        new_node->data = current->data;
        new_node->next = NULL;

        if (new_head == NULL) {
            new_head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        current = current->next;
    }

    return new_head;

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

/** result calculations */

double mean(const double arr[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}

double standard_deviation(const double arr[], int size, double mean_value) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += pow(arr[i] - mean_value, 2);
    }
    return sqrt(sum / size);
}

void print_results(
        double serial_results[3][ROUNDS],
        double parallel_one_mutex_results[3][4][ROUNDS],
        double parallel_rwlock_results[3][4][ROUNDS]
) {


    for (int i = 0; i < 3; i++) {
        printf("\nCase %d\n", i);
        printf("Percentages: %f, %f, %f\n", cases[i][0], cases[i][1], cases[i][2]);
        printf("Serial\n");


        double mean_value = mean(serial_results[i], ROUNDS);
        double std_value = standard_deviation(serial_results[i], ROUNDS, mean_value);
        printf("Mean: %f, Standard Deviation: %f\n", mean_value, std_value);

        printf("Parallel One Mutex\n");
        for (int j = 0; j < 4; j++) {
            mean_value = mean(parallel_one_mutex_results[i][j], ROUNDS);
            std_value = standard_deviation(parallel_one_mutex_results[i][j], ROUNDS, mean_value);
            printf("Thread count : %d,Mean: %f, Standard Deviation: %f\n", thread_counts[j], mean_value, std_value);
        }

        printf("Parallel One RWLock\n");
        for (int j = 0; j < 4; j++) {
            mean_value = mean(parallel_rwlock_results[i][j], ROUNDS);
            std_value = standard_deviation(parallel_rwlock_results[i][j], ROUNDS, mean_value);
            printf("Thread count : %d,Mean: %f, Standard Deviation: %f\n", thread_counts[j], mean_value, std_value);
        }

    }
}

void write_to_csv(
        double serial_results[3][ROUNDS],
        double parallel_one_mutex_results[3][4][ROUNDS],
        double parallel_rwlock_results[3][4][ROUNDS]
) {
    // Open the CSV file for writing (create a new file or overwrite an existing one)
    FILE *csv_file;
    csv_file = fopen("D:\\7th Sem\\Concurrent Programming\\Labs\\Lab 1\\cp-lab\\results.csv", "w");
    printf("Writing results to results.csv\n");
    if (csv_file == NULL) {
        perror("Failed to open results.csv for writing");
        exit(1);
    }

    // Write header row
    fprintf(csv_file, "Case No,Member Percentage,Insert Percentage,Delete Percentage,Mean Value,Standard Deviation,Method,No. of Threads,No. of Rounds\n");

    for (int i = 0; i < 3; i++) {

            double mean_value, std_value;
            // Serial
            mean_value = mean(serial_results[i], ROUNDS);
            std_value = standard_deviation(serial_results[i], ROUNDS, mean_value);
            fprintf(csv_file, "%d, %f, %f, %f, %f, %f,Serial, 1, %d\n", i, cases[i][0], cases[i][1], cases[i][2], mean_value, std_value, ROUNDS);
            for (int j = 0; j < 4; j++) {
                mean_value = mean(parallel_one_mutex_results[i][j], ROUNDS);
                std_value = standard_deviation(parallel_one_mutex_results[i][j], ROUNDS, mean_value);
                fprintf(csv_file, "%d, %f, %f, %f, %f, %f,%s, %d, %d\n", i, cases[i][0], cases[i][1], cases[i][2], mean_value, std_value, "Parallel One Mutex", thread_counts[j], ROUNDS);
            }
            for (int j = 0; j < 4; j++) {
                mean_value = mean(parallel_rwlock_results[i][j], ROUNDS);
                std_value = standard_deviation(parallel_rwlock_results[i][j], ROUNDS, mean_value);
                fprintf(csv_file, "%d, %f, %f, %f, %f, %f,%s, %d, %d\n", i, cases[i][0], cases[i][1], cases[i][2], mean_value, std_value, "Parallel RWLock", thread_counts[j], ROUNDS);
            }
    }

    // Close the CSV file when you're done writing the results
    fclose(csv_file);
}

