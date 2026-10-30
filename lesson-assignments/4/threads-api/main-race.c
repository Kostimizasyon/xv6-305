#include <stdio.h>

#include "common_threads.h"

int balance = 0;
pthread_mutex_t mutex;

// will cause a race cond

// 1st iteration of removing one of the balance acess will remove race if we delete it from the worker otherwise it wont change

// 2nd iteration of only locking one is also fucked

// 3rd iteration locking both is fine

void* worker(void* arg) {
    Mutex_lock(&mutex);
    balance++; // unprotected access 
    Mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char *argv[]) {
    Mutex_init(&mutex);
    pthread_t p;
    Pthread_create(&p, NULL, worker, NULL);

    Mutex_lock(&mutex);
    balance++; // unprotected access
    Mutex_unlock(&mutex);

    Pthread_join(p, NULL);

    printf("%d", balance);
    return 0;
}
