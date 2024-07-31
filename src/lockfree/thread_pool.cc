
#include "mpmc.h"
#include <thread>
#include <stop_token>
#include <iostream>
#include <chrono>

/*
Start a thread pool which has a number of always running worker threads which pick up tasks from the MPMC queue.
This is not lock free as we use condition variables to suspend the thread.
*/

constexpr int NUM_THREADS = 4;

// worker runner
void run(std::stop_token token, MpmcQueue<std::function<void()>>& queue, std::condition_variable& cv, std::mutex& mu) {
    
    std::unique_lock lock(mu);
    while (!token.stop_requested()) {
        cv.wait(lock, [&]{return token.stop_requested() || queue.get_size() > 0; });

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
    std::condition_variable cv;
    std::mutex mu;

public:
    ThreadPool() {}
    ~ThreadPool() {
        stop();
    }

    // start threadpool
    void startup() {
        for (int i = 0; i < NUM_THREADS; i++) {
            workers.emplace_back(run, std::ref(mpmc), std::ref(cv), std::ref(mu));
        }
    }

    void enqueue(std::function<void()> task) {
        mpmc.push(std::move(task));
        cv.notify_all();
    }

    // stop the thread pool
    void stop() {
        std::cout << "requesting stop\n";
        for (int i = 0; i < NUM_THREADS; i++) {
            workers[i].request_stop();
        }
        cv.notify_all();
    }
};

void print() {
    std::cout << "Hello world!\n";
}

int main () {
    ThreadPool tp;
    tp.startup();
    tp.enqueue(print);
    tp.enqueue(print);
}