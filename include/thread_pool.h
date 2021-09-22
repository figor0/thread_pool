#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <future>

namespace abstract { namespace from { namespace thread {

std::ostream& reader();

template<class T,
         class Container = std::vector<T>,
         class Compare = std::less<typename Container::value_type>>
class ThreadSafePriorityQueue
{
public:
    typedef typename Container::const_reference const_reference;
	void push(const T& value)
	{
		std::lock_guard<std::mutex> lock(m_sync);
		m_notSafeQueue.push(value);
	}
	T pop()
	{
		std::lock_guard<std::mutex> lock(m_sync);
		auto value = m_notSafeQueue.top();
		m_notSafeQueue.pop();
		return value;
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m_sync);
		return m_notSafeQueue.empty();
	}
private:
    std::priority_queue<T, Container, Compare> m_notSafeQueue;
    mutable std::mutex m_sync;
};

struct Operation
{
    int32_t priority;
    std::function<void ()> function;
};

bool operator<(const Operation& left, const Operation& right);

class ThreadPool
{
public:
    class Worker
    {
    public:
        Worker(ThreadPool& pool, const int id);
        void operator()();
    private:
        int m_id;
        ThreadPool& m_pool;
    };
    ThreadPool();
    ThreadPool(const ThreadPool& thread_pool) = delete;
    ThreadPool& operator=(const ThreadPool& thread_pool) = delete;
    void stop();
    bool start(const size_t& threads_amount);
    size_t size();
    template<typename Callable, typename ...Args>
    auto submit(const int32_t priority,
                Callable&& f,
				Args&&... args) -> std::shared_ptr<std::packaged_task<decltype (f(args...))()>>
    {
        auto func = std::bind(std::forward<Callable>(f), std::forward<Args>(args)...);
		auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        std::function<void()> wrapper_func = [task_ptr]()
        {
            task_ptr->operator()();
        };

        m_operations.push({priority, wrapper_func});

        m_cv.notify_one();

		return task_ptr;
    }
private:
    ThreadSafePriorityQueue<Operation> m_operations;
    std::vector<std::thread> m_threads;
    std::condition_variable m_cv;
    std::mutex m_conditional_sync;
    bool m_stoped;
};

}
}
}
