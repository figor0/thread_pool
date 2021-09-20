#include <thread_pool.h>

namespace abstract { namespace from { namespace thread {

bool operator<(const Operation &left, const Operation &right)
{
    return left.priority < right.priority;
}

ThreadPool::ThreadPool() { }

void ThreadPool::stop(){
    m_stoped = true;
    m_cv.notify_all();
    for (auto& thread: m_threads)
    {
        if (thread.joinable())
            thread.join();
    }
}

bool ThreadPool::start(const size_t &threads_amount)
{
    if (threads_amount == 0 )
        return false;

    m_threads.reserve(threads_amount);
    for (size_t i = 0; i < m_threads.size(); i++)
    {
        m_threads.emplace_back(std::thread(Worker(*this, i)));
    }
    return true;
}

size_t ThreadPool::size(){
    return m_threads.size();
}

ThreadPool::Worker::Worker(ThreadPool &pool, const int id)
    : m_id(id), m_pool(pool)
{
}

void ThreadPool::Worker::operator()()
{
    std::function<void()> func;
    bool success = false;
    while(!m_pool.m_stoped){
        std::unique_lock<std::mutex> lock(m_pool.m_conditional_sync);
        if (m_pool.m_operations.empty()){
            m_pool.m_cv.wait(lock);
        }
        func = m_pool.m_operations.pop().function;
        func();
    }
}

template<class T, class Container, class Compare>
void ThreadSafePriorityQueue<T, Container, Compare>::push(const T &value)
{
    std::lock_guard<std::mutex> lock(m_sync);
    m_notSafeQueue.push(value);
}

template<class T, class Container, class Compare>
T ThreadSafePriorityQueue<T, Container, Compare>::pop()
{
    std::lock_guard<std::mutex> lock(m_sync);
    auto value = m_notSafeQueue.top();
    m_notSafeQueue.pop();
    return value;
}

template<class T, class Container, class Compare>
bool ThreadSafePriorityQueue<T, Container, Compare>::empty() const
{
    std::lock_guard<std::mutex> lock(m_sync);
    return m_notSafeQueue.empty();
}

}
}
}
