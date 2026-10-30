#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common_threads.h"

typedef struct __rwlock_t {
    sem_t lock;      // Binary semaphore (mutex) used ONLY to protect the 'readers' counter from race conditions
    sem_t writelock; // Binary semaphore that locks out writers (and waits for an active writer to finish)
    int readers;     // Tracks the exact number of readers currently inside the critical section
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
    rw->readers = 0;            // Initialize active reader count to 0 since no threads are reading yet
    Sem_init(&rw->lock, 1);     // Initialize 'lock' to 1 so the first reader to arrive can update 'readers'
    Sem_init(&rw->writelock, 1);// Initialize 'writelock' to 1 so either a writer or the first reader can grab it
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    Sem_wait(&rw->lock);            // Acquire 'lock' so only one reader can modify 'rw->readers' at a time
    rw->readers++;                  // Register that one more reader wants to enter the critical section
    if (rw->readers == 1) {         // Check if this thread is the VERY FIRST active reader
        Sem_wait(&rw->writelock);   // If first reader: wait for any active writer to finish, then lock out future writers
    }
    Sem_post(&rw->lock);            // Release 'lock' so other readers can increment 'rw->readers' and read simultaneously
}

void rwlock_release_readlock(rwlock_t *rw) {
    Sem_wait(&rw->lock);            // Acquire 'lock' to safely decrement the shared 'rw->readers' counter
    rw->readers--;                  // This reader is done reading, so decrease the active reader count by 1
    if (rw->readers == 0) {         // Check if this thread is the VERY LAST active reader leaving
        Sem_post(&rw->writelock);   // If last reader: release 'writelock' so a waiting writer is finally allowed in
    }
    Sem_post(&rw->lock);            // Release 'lock' now that we are done updating 'rw->readers'
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    Sem_wait(&rw->writelock);       // Wait until 'writelock' is free (meaning 0 active readers and 0 active writers), then grab it
}

void rwlock_release_writelock(rwlock_t *rw) {
    Sem_post(&rw->writelock);       // Release 'writelock' so waiting writers or the first waiting reader can proceed
}

//
// Don't change the code below (just use it!)
// 

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        rwlock_acquire_readlock(&lock);
        printf("read %d\n", value);
        rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        rwlock_acquire_writelock(&lock);
        value++;
        printf("write %d\n", value);
        rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
        Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
        Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
        Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
        Pthread_join(pw[i], NULL);

    printf("end: value %d\n", value);

    return 0;
}
