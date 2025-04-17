#include "../include/Task.h"
#include "../include/Scheduler.h"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

// Sample task function
void sampleTask(int id) {
    cout << "Task " << id << " executed on thread " << std::this_thread::get_id() << std::endl;
}

int main() {
    // Read number of threads directly from stdin
    int numThreads;
    cin >> numThreads;

    if (numThreads <= 0) {
        std::cerr << "Invalid number of scheduler threads specified.\n";
        return 1;
    }

    // Initialize adaptive scheduler with specified threads
    AdaptiveScheduler Scheduler(numThreads);
    Scheduler.start();

    // Read number of tasks
    int numTasks;
    cin >> numTasks;

    if (numTasks <= 0) {
        std::cerr << "Invalid number of tasks specified.\n";
        return 1;
    }

    // Read and submit each task
    for (int i = 0; i < numTasks; ++i) {
        int taskId, taskPriority;
        cin >> taskId >> taskPriority;

        if (cin.fail()) {
            std::cerr << "Invalid task details specified.\n";
            return 1;
        }

        // Submit task with priority
        Scheduler.submit(Task([taskId]() { sampleTask(taskId); }, taskPriority));
    }

    // Allow time for tasks to execute
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Stop scheduler
    Scheduler.stop();

    return 0;
}