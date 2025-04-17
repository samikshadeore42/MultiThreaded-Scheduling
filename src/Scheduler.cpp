#include "../include/Scheduler.h"
#include <iostream>
#include <chrono>

AdaptiveScheduler::AdaptiveScheduler(int n) : numThreads(n), localQueues(n), done(false) {}

AdaptiveScheduler::~AdaptiveScheduler() {
    stop();
}

void AdaptiveScheduler::start() {
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back(&AdaptiveScheduler::worker, this, i);
    }
}

void AdaptiveScheduler::stop() {
    done = true;
    cv.notify_all();
    for (auto& w : workers) {
        if (w.joinable()) w.join();
    }
}

void AdaptiveScheduler::submit(Task newtask) {
    std::lock_guard<std::mutex> lock(globalMutex);
    globalQueue.push(newtask);
    cv.notify_one();
}

void AdaptiveScheduler::worker(int index) {
    while (!done) {
        Task newtask([] {}, -1);
        bool found = false;

        {
            std::unique_lock<std::mutex> lock(globalMutex);
            cv.wait(lock, [&] { return done || !globalQueue.empty(); });

            if (!globalQueue.empty()) {
                newtask = globalQueue.top();
                globalQueue.pop();
                found = true;
            }
        }

        if (!found && !localQueues[index].empty()) {
            newtask = localQueues[index].front();
            localQueues[index].pop_front();
            found = true;
        }

        // Try stealing
        if (!found) {
            for (int i = 0; i < numThreads; ++i) {
                if (i != index && !localQueues[i].empty()) {
                    newtask = localQueues[i].back();
                    localQueues[i].pop_back();
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            std::cout << "Executing task on thread " << std::this_thread::get_id() << "\n";
            newtask.execute();
        }
    }
}
