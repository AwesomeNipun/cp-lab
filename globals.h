#ifndef SOURCE_CODE_GLOBALS_H
#define SOURCE_CODE_GLOBALS_H
#include <pthread.h> // Include the pthread header

extern int *op_array;
extern struct list_node *dup_head;
extern int n;
extern int m;
extern pthread_mutex_t mutex;
extern pthread_rwlock_t rwlock;
#endif //SOURCE_CODE_GLOBALS_H
