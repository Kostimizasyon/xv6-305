#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

struct Buffer {
    static constexpr size_t capacity = 1024;

    std::queue<uint32_t> q;
    bool done = false;

    std::mutex m;
    std::condition_variable not_full;   // producers wait here
    std::condition_variable not_empty;  // consumers wait here
};

constexpr uint32_t items_per_producer = 100000;
std::atomic<uint64_t> total{0};

void producer(Buffer& buf)
{
    for (uint32_t i = 1; i <= items_per_producer; ++i) {
        std::unique_lock lock(buf.m);
        buf.not_full.wait(lock, [&] { return buf.q.size() < buf.capacity; });
        buf.q.push(i);
        lock.unlock();
        buf.not_empty.notify_one();
    }
}

void consumer(Buffer& buf)
{
    uint64_t local = 0;
    for (;;) {
        std::unique_lock lock(buf.m);
        buf.not_empty.wait(lock, [&] { return !buf.q.empty() || buf.done; });

        if (buf.q.empty())  // done and fully drained
            break;

        uint32_t v = buf.q.front();

        buf.q.pop();

        lock.unlock();

        buf.not_full.notify_one();

        local += v;  // "process" the item outside the lock
    }
    total += local;
}

int main(int argc, char** argv)
{
    assert(argc == 3);  // usage: ./threads <producers> <consumers>

    const int prod_count = std::stoi(argv[1]);
    const int cons_count = std::stoi(argv[2]);

    Buffer buf;  // declared BEFORE the threads so it outlives them

    std::vector<std::jthread> prods;
    std::vector<std::jthread> cons;
    prods.reserve(prod_count);
    cons.reserve(cons_count);

    for (int i = 0; i < prod_count; ++i)
        prods.emplace_back(producer, std::ref(buf));

    for (int i = 0; i < cons_count; ++i)
        cons.emplace_back(consumer, std::ref(buf));

    // 1. wait for all producers to finish
    for (auto& t : prods) t.join();

    // 2. tell consumers no more data is coming
    {
        std::lock_guard lock(buf.m);
        buf.done = true;
    }
    buf.not_empty.notify_all();

    // 3. wait for consumers to drain the queue and exit
    for (auto& t : cons) t.join();

    const uint64_t n = items_per_producer;
    const uint64_t expected = prod_count * (n * (n + 1) / 2);
    std::cout << "total=" << total << " expected=" << expected << "\n";
}
