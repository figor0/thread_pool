#include <thread_pool.h>
#include <iostream>

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
	m_threads.clear();
}

bool ThreadPool::start(const size_t &threads_amount)
{
	if (threads_amount == 0 || m_threads.size() != 0)
        return false;

    m_threads.reserve(threads_amount);
	for (size_t i = 0; i < threads_amount; i++)
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
	std::cout << "Worker with id " << m_id << " begin to work" << std::endl;
    std::function<void()> func;
    bool success = false;
    while(!m_pool.m_stoped){
        std::unique_lock<std::mutex> lock(m_pool.m_conditional_sync);
        if (m_pool.m_operations.empty()){
            m_pool.m_cv.wait(lock);
        }
		if (m_pool.m_stoped)
			break;
		if (!m_pool.m_operations.empty())
			m_pool.m_operations.pop().function();
    }
	std::cout << "Worker with id " << m_id << " end" << std::endl;
}

std::ostream &reader(){
	static std::mutex cout_mutex;
	std::lock_guard<std::mutex> lock(cout_mutex);
	return std::cout;
}

}
									}
				   }
