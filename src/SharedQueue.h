#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class SharedQueue
{
  private:
    std::queue<T> mQueue;
    mutable std::mutex mMutex;
    std::condition_variable mCondition;

  public:
    void push(const T& element);
    void pop();
    const T& front() const;
    T& front();
    bool empty() const;
};

template<typename T>
inline void SharedQueue<T>::push(const T& element)
{
    std::unique_lock<std::mutex> mlock(mMutex);
    mQueue.push(element);
    mCondition.notify_one();
}

template<typename T>
inline void SharedQueue<T>::pop()
{
    std::unique_lock<std::mutex> mlock(mMutex);

    while (mQueue.empty())
        mCondition.wait(mlock);

    mQueue.pop();
}

template<typename T>
inline const T& SharedQueue<T>::front() const
{
    std::unique_lock<std::mutex> mlock(mMutex);

    while (mQueue.empty())
        mCondition.wait(mlock);

    return mQueue.front();
}

template<typename T>
inline T& SharedQueue<T>::front()
{
    std::unique_lock<std::mutex> mlock(mMutex);

    while (mQueue.empty())
        mCondition.wait(mlock);

    return mQueue.front();
}

template<typename T>
inline bool SharedQueue<T>::empty() const
{
    std::unique_lock<std::mutex> mlock(mMutex);
    bool e = mQueue.empty();
    return e;
}
