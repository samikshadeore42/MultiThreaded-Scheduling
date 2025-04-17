#pragma once
#include "../include/Task.h"

// Constructor to initialize Task with ID, priority, and function
// /*Task::Task(int id, int priority, std::function<void()> func)
//     : id(id), priority(priority), func(std::move(func)), creationTime(std::chrono::high_resolution_clock::now()) {}*/

Task::Task(std::function<void()> f, int p) 
    : id(-1), delay(0), func(std::move(f)), priority(p),
      creationTime(std::chrono::high_resolution_clock::now()) {} 

Task::Task(int id, int delay, std::function<void()> func, int priority)
    : id(id), delay(delay), func(std::move(func)), priority(priority),
      creationTime(std::chrono::high_resolution_clock::now()) {}

bool Task::isReady() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - creationTime).count();
    return elapsed >= delay;
}

// Get task ID
int Task::getId() const {
    return id;
}

// Get task priority
int Task::getPriority() const {
    return priority;
}

// Get task creation time
std::chrono::time_point<std::chrono::high_resolution_clock> Task::getCreationTime() const {
    return creationTime;
}

// // Execute the task function
// void Task::execute() const {
//     func();
// }

// Comparison operator to determine task priority for scheduling
bool Task::operator>(const Task& other) const {
    // Tasks with higher priority should come first; if equal, older tasks get priority
    if (priority == other.priority) {
        return creationTime < other.creationTime;
    }
    return priority > other.priority;
}
