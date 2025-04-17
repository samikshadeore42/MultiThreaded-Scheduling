#ifndef TASK_H
#define TASK_H

#include <functional>
#include <chrono>

class Task {
public:
    Task(std::function<void()> f, int p);
    Task(int id, int delay, std::function<void()> f, int p);
    
    bool isReady() const;
    int getId() const;
    int getPriority() const;
    void execute() const { func(); }
    bool operator>(const Task& other) const;
    bool operator<(const Task& other) const {
        return !(*this > other); 
    }
    

    std::chrono::time_point<std::chrono::high_resolution_clock> getCreationTime() const;

private:
    int id;              // Single declaration
    int delay;           // Single declaration
    std::function<void()> func;
    int priority;
    std::chrono::time_point<std::chrono::high_resolution_clock> creationTime;
};

#endif // TASK_H
