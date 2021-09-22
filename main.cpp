#include <iostream>
#include <thread_pool.h>
#include <string>
#include <vector>
#include <sstream>

namespace aft = abstract::from::thread;

int64_t findBiggerDelimeter(const int64_t value)
{
	int64_t result = 100;
	return result;
}


void printDelimeter(std::shared_ptr<std::packaged_task<int64_t ()>> task_ptr)
{
	std::cout << task_ptr->get_future().get() << '\n';
}

int main()
{
	aft::ThreadPool thread_pool;
	thread_pool.start(1);
	std::string command;
	bool stopeable = false;
	bool restrtable = true;
	size_t current_size = 1;

	std::future<std::future<int64_t>> future;
	while(true)
	{
		std::cout << "Input command" << std::endl;
		std::getline( std::cin, command);
		std::cout << "Command " + command << std::endl;
		if (command == "exit"){
			stopeable = true;
			restrtable = false;
		} else if (command == "restart"){
			stopeable = true;
			restrtable = true;
		} else {
			std::istringstream reader(command);
			int64_t value = 0;
			int32_t priority = 0;

			reader >> value >> priority;

			if (!reader.fail())
			{
				auto result = thread_pool.submit(priority,
												 findBiggerDelimeter,
												 value);
				thread_pool.submit(priority, printDelimeter, result);
			} else {
				std::cout << "read data from cin failed" << std::endl;
			}
		}
		if (stopeable){
			current_size = thread_pool.size();
			thread_pool.stop();
		}
		if ( restrtable ) {
			thread_pool.start(current_size*2);
		} else if (stopeable){
			break;
		}
	}
    return 0;
}
