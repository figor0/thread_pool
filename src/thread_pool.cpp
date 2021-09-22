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

    m_stoped = false;
    m_threads.reserve(threads_amount);
	for (size_t i = 0; i < threads_amount; i++)
    {
		m_threads.emplace_back(std::thread(Worker(*this, i)));
    }
    return m_stoped;
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
    std::string begin_message = "Worker with id " + std::to_string(m_id) + " begin to work";
    print(std::cout, begin_message);
    std::function<void()> func;
    bool success = false;
    while(!m_pool.m_stoped){
        std::unique_lock<std::mutex> lock(m_pool.m_conditional_sync);

        std::string msg_start_task = "Worker " + std::to_string(m_id) + "begin task";
        std::string msg_end_task = "Worker " + std::to_string(m_id) + "end task";

        if (m_pool.m_operations.empty()){
            m_pool.m_cv.wait(lock);
        }

		if (m_pool.m_stoped)
			break;
        if (!m_pool.m_operations.empty()){
            print(std::cout, msg_start_task);
			m_pool.m_operations.pop().function();
            print(std::cout, msg_end_task);
        }
    }
}

void print(std::ostream& printer, const std::string& message){
	static std::mutex cout_mutex;
	std::lock_guard<std::mutex> lock(cout_mutex);
    printer << message << std::endl;
}

}
									}
				   }
