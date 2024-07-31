
#include "mpmc.h"
#include <thread>
#include <stop_token>
#include <iostream>

/*
Start a thread pool which has a number of always running worker threads which pick up tasks from the MPMC queue.
*/

constexpr int NUM_THREADS = 4;

// worker runner
void run(std::stop_token token, MpmcQueue<std::function<void()>>& queue) {
    while (!token.stop_requested()) {
        if (queue.get_size() > 0) {
            auto callable = queue.pop();
            if (callable) (callable.value())();
        }
    }
}

class ThreadPool {
private:
    std::vector<std::jthread> workers;
    MpmcQueue<std::function<void()>> mpmc;

public:
    ThreadPool() {}
    ~ThreadPool() {
        stop();
    }

    // start threadpool
    void startup() {
        for (int i = 0; i < NUM_THREADS; i++) {
            workers.emplace_back(run, std::ref(mpmc));
        }
    }

    void enqueue(std::function<void()> callable) {
        mpmc.push(callable);
    }

    // stop the thread pool
    void stop() {
        std::cout << "requesting stop\n";
        for (int i = 0; i < NUM_THREADS; i++) {
            workers[i].request_stop();
        }
    }
};

void print() {
    std::cout << "I'm printing mom!\n";
}

int main () {
    ThreadPool tp;

    tp.startup();
    tp.enqueue(print);
    std::cout << "Got here\n";
}