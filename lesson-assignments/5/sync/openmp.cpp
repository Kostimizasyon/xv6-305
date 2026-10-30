#include <omp.h>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>

constexpr size_t capacity = 1024;
constexpr uint32_t items_per_producer = 100000;

int main(int argc, char** argv)
{
    const int P = std::stoi(argv[1]);
    const int C = std::stoi(argv[2]);

    std::queue<uint32_t> q;
    int producers_done = 0;
    uint64_t total = 0;

    #pragma omp parallel num_threads(P + C)
    {
        const int id = omp_get_thread_num();

        if (id < P) {                                   // producer
            for (uint32_t i = 1; i <= items_per_producer; ++i) {
                bool pushed = false;
                while (!pushed) {                       // spin until there's room
                    #pragma omp critical(queue)
                    {
                        if (q.size() < capacity) {
                            q.push(i);
                            pushed = true;
                        }
                    }
                }
            }
            #pragma omp atomic
            ++producers_done;
        }
        else {                                          // consumer
            uint64_t local = 0;
            for (;;) {
                int d;
                #pragma omp atomic read
                d = producers_done;                     // read BEFORE checking the queue

                bool got = false;
                uint32_t v = 0;
                #pragma omp critical(queue)
                {
                    if (!q.empty()) {
                        v = q.front();
                        q.pop();
                        got = true;
                    }
                }

                if (got)         local += v;            // process outside the lock
                else if (d == P) break;                 // all producers done AND queue empty
            }
            #pragma omp atomic
            total += local;
        }
    }

    const uint64_t n = items_per_producer;
    std::cout << "total=" << total << " expected=" << P * (n * (n + 1) / 2) << "\n";
}
