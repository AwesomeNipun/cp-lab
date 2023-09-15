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
#include "LinkedList.h"

double cases[3][3] = {
    {0.99, 0.005, 0.005},
    {0.9, 0.05, 0.05},
    {0.5, 0.25, 0.25}};

int thread_counts[4] = {1, 2, 4, 8};
int n = 1000;
int m = 10000;
int ROUNDS = 400;
struct list_node *head = NULL;
int *op_array;
struct list_node *dup_head = NULL;

pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

double mean(const double arr[], int size);

double standard_deviation(const double arr[], int size, double mean_value);

int required_sample_size(double std_value, double mean_value);

void print_results(
    double serial_results[3][ROUNDS],
    double parallel_one_mutex_results[3][4][ROUNDS],
    double parallel_one_rwlock_results[3][4][ROUNDS]);

void write_to_csv(
    double serial_results[3][ROUNDS],
    double parallel_one_mutex_results[3][4][ROUNDS],
    double parallel_rwlock_results[3][4][ROUNDS]);

int main()
{

    double serial_results[3][ROUNDS];
    double parallel_one_mutex_results[3][4][ROUNDS];
    double parallel_rwlock_results[3][4][ROUNDS];

    pthread_mutex_init(&mutex, NULL);   // Initialize the mutex
    pthread_rwlock_init(&rwlock, NULL); // Initialize the read-write lock

    initialize_list(&head, n);

    for (int i = 0; i < 3; i++)
    {
        printf("\nCase %d\n", i);
        for (int r = 0; r < ROUNDS; r++)
        {
            if (r == 50 || r == 100 || r == 150 || r == 200 || r == 250 || r == 300 || r == 350) {
                printf("Round %d\n", r);
            }
            //            printf("Round %d\n", r);
            op_array = createArray(m, cases[i][0], cases[i][1], cases[i][2]);

            dup_head = copyList(head);

            serial_results[i][r] = serial_impl();
            // destroy the linked list
            destroyLinkedList(dup_head);
            dup_head = NULL;

            for (int j = 0; j < 4; j++)
            {
                dup_head = copyList(head);
                parallel_one_mutex_results[i][j][r] = parallel_with_one_mutex(thread_counts[j]);
                // destroy the linked list
                destroyLinkedList(dup_head);
                dup_head = NULL;

                dup_head = copyList(head);
                parallel_rwlock_results[i][j][r] = parallel_with_rwlock(thread_counts[j]);
                // destroy the linked list
                destroyLinkedList(dup_head);
                dup_head = NULL;

            }
        }
    }

    // destroy the linked list
    destroyLinkedList(head);

    // Destroy the mutex and read-write lock when done
    pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&rwlock);

    // destroy the op_array
    free(op_array);

    // print_results(serial_results, parallel_one_mutex_results, parallel_rwlock_results);
    write_to_csv(serial_results, parallel_one_mutex_results, parallel_rwlock_results);
}

/** result calculations */

double mean(const double arr[], int size)
{
    double sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum / size;
}

double standard_deviation(const double arr[], int size, double mean_value)
{
    double sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        sum += pow(arr[i] - mean_value, 2);
    }
    return sqrt(sum / size);
}

int required_sample_size(double std_value, double mean_value)
{
    return (int)ceil(pow(100 * 1.96 * std_value / (5 * mean_value), 2));
}

void write_to_csv(
    double serial_results[3][ROUNDS],
    double parallel_one_mutex_results[3][4][ROUNDS],
    double parallel_rwlock_results[3][4][ROUNDS])
{
    // Open the CSV file for writing (create a new file or overwrite an existing one)
    FILE *csv_file;
    csv_file = fopen("../results/results.csv", "w");
    printf("Writing results to results.csv\n");
    if (csv_file == NULL)
    {
        perror("Failed to open results.csv for writing");
        exit(1);
    }

    // Write header row
    fprintf(csv_file, "Case No,Member Percentage,Insert Percentage,Delete Percentage,Mean Value,Standard Deviation,Method,No. of Threads,Tested Rounds, Required Rounds\n");

    for (int i = 0; i < 3; i++)
    {

        int required_rounds;
        double mean_value, std_value;
        // Serial
        mean_value = mean(serial_results[i], ROUNDS);
        std_value = standard_deviation(serial_results[i], ROUNDS, mean_value);
        required_rounds = required_sample_size(std_value, mean_value);
        fprintf(csv_file, "%d, %f, %f, %f, %f, %f,Serial, 1, %d, %d\n", i, cases[i][0], cases[i][1], cases[i][2], mean_value, std_value, ROUNDS, required_rounds);

        for (int j = 0; j < 4; j++)
        {
            mean_value = mean(parallel_one_mutex_results[i][j], ROUNDS);
            std_value = standard_deviation(parallel_one_mutex_results[i][j], ROUNDS, mean_value);
            required_rounds = required_sample_size(std_value, mean_value);
            fprintf(csv_file, "%d, %f, %f, %f, %f, %f,%s, %d, %d, %d\n", i, cases[i][0], cases[i][1], cases[i][2], mean_value, std_value, "Parallel One Mutex", thread_counts[j], ROUNDS, required_rounds);
        }

        for (int j = 0; j < 4; j++)
        {
            mean_value = mean(parallel_rwlock_results[i][j], ROUNDS);
            std_value = standard_deviation(parallel_rwlock_results[i][j], ROUNDS, mean_value);
            required_rounds = required_sample_size(std_value, mean_value);
            fprintf(csv_file, "%d, %f, %f, %f, %f, %f,%s, %d, %d, %d\n", i, cases[i][0], cases[i][1], cases[i][2], mean_value, std_value, "Parallel RWLock", thread_counts[j], ROUNDS, required_rounds);
        }
    }

    // Close the CSV file when you're done writing the results
    fclose(csv_file);
}
