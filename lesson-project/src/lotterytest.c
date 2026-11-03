#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "pstat.h"

void spin() {
    volatile int x = 0;
    while (1) {
        x = x + 1;
    }
}

int main(int argc, char *argv[]) {
    int tickets[3] = {30, 20, 10};
    int pids[3];
    struct pstat st;

    // Give parent high tickets so it can always wake up and log immediately
    settickets(100);

    for (int i = 0; i < 3; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            settickets(tickets[i]);
            spin(); // Infinite CPU work
        }
    }

    printf(1, "time,procA_30,procB_20,procC_10\n");

    // Sample 50 times, sleeping 10 ticks between each sample
    for (int t = 1; t <= 50; t++) {
        sleep(10); // Sleep for 10 timer ticks
        if (getpinfo(&st) != 0) {
            printf(1, "getpinfo failed\n");
            break;
        }

        int ticksA = 0, ticksB = 0, ticksC = 0;
        for (int j = 0; j < NPROC; j++) {
            if (st.inuse[j]) {
                if (st.pid[j] == pids[0]) ticksA = st.ticks[j];
                if (st.pid[j] == pids[1]) ticksB = st.ticks[j];
                if (st.pid[j] == pids[2]) ticksC = st.ticks[j];
            }
        }
        printf(1, "%d,%d,%d,%d\n", t * 10, ticksA, ticksB, ticksC);
    }

    // Clean up children
    for (int i = 0; i < 3; i++) {
        kill(pids[i]);
        wait();
    }
    exit();
}
