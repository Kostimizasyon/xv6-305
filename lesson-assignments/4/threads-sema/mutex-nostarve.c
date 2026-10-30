#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

#define N 64   // must be >= max number of threads

typedef struct ns_mutex_t {
    sem_t slots[N];   // slots[0] starts at 1, the rest at 0
    sem_t guard;      // protects next_ticket
    int next_ticket;  // next ticket to hand out
    int serving;      // ticket currently holding the lock
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    for (int i = 0; i < N; i++)
        Sem_init(&m->slots[i], i == 0);
    Sem_init(&m->guard, 1);
    m->next_ticket = 0;
    m->serving = 0;
}

void ns_mutex_acquire(ns_mutex_t *m) {
    Sem_wait(&m->guard);
    int t = m->next_ticket++;
    Sem_post(&m->guard);
    Sem_wait(&m->slots[t % N]);
}

void ns_mutex_release(ns_mutex_t *m) {
    m->serving++;                          // only the lock holder touches this
    Sem_post(&m->slots[m->serving % N]);
}
