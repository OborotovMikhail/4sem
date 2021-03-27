#pragma once

#include <queue>
#include <mutex>

// A threadsafe-queue
template <class T>
class SafeQueue
{
public:
  // Add an element to the queue.
  void enqueue(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(m);
    return q.empty();
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is available.
  // For simplicity, assume that queue isn't empty
  T dequeue(void)
  {
    std::lock_guard<std::mutex> lock(m);

    T val = q.front();
    q.pop();
    return val;
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
};
