#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "Task.h"
#include <iostream>

// struct Task {
//     std::function<void()> func;
//     int priority;

//     Task(std::function<void()> f, int p) : func(std::move(f)), priority(p) {}

//     // For priority queue (higher priority = earlier)
//     bool operator<(const Task& other) const {
//         return priority < other.priority;
//     }
// };

class AdaptiveScheduler {
public:
    AdaptiveScheduler(int numThreads);
    ~AdaptiveScheduler();

    void submit(Task task);
    void start();
    void stop();

private:
    void worker(int index);

    std::vector<std::thread> workers;
    std::vector<std::deque<Task>> localQueues;
    std::mutex globalMutex;
    std::priority_queue<Task, std::vector<Task>, std::greater<Task>> globalQueue;
    std::condition_variable cv;
    std::atomic<bool> done;
    int numThreads;

    
};
