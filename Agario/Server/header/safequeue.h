#pragma once

#include <queue>
#include <mutex>

// A threadsafe-queue
template <class T>
class SafeQueue
{
    std::queue<T> q; // Queue
    mutable std::mutex m; // Mutex

public:
    // Add an element to the queue
    void enqueue(T t);

    // Take alement from the queue
    bool empty() const;

    // Get the "front"-element
    // If the queue is empty, wait till a element is available
    // For simplicity, assume that queue isn't empty
    T dequeue(void);
};