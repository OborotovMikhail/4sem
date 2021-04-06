// Does not seem to work when in cpp, so moved to header file

/*
#pragma once

#include "safequeue.h"

template<class T>
void SafeQueue<T>::enqueue(T t)
{
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
}

template<class T>
bool SafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m);
    return q.empty();
}

template<class T>
T SafeQueue<T>::dequeue(void)
{
    std::lock_guard<std::mutex> lock(m);

    T val = q.front();
    q.pop();
    return val;
}
*/
