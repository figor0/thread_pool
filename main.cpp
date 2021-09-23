#include <iostream>
#include <thread_pool.h>
#include <string>
#include <vector>
#include <sstream>

#include <atkin_sieve.h>

namespace aft = abstract::from::thread;

std::vector<unsigned> calculateSimpleNumbers(const unsigned limit)
{
    std::vector<unsigned> result;

    //\todo нужно понять, сколько можно заранее резервировать в векторе
    auto numbers = atkin_sieve::getPrimesUpTo(limit);

    for(unsigned number = 2u; number < limit; ++number)
    {
        if ( numbers[number] == true)
            result.push_back(number);
    }
    return result;
}

unsigned findBiggerDelimeter(const unsigned value)
{
    unsigned result = 1;
    auto simple_numbers = calculateSimpleNumbers(value);
    for (const auto sim_number: simple_numbers)
    {
        if (value % sim_number == 0){
            result = sim_number;
        }
    }
    return result;
}


void printDelimeter(std::shared_ptr<std::packaged_task<unsigned ()>> task_ptr)
{
    aft::print(std::cout, std::to_string(task_ptr->get_future().get()));
}



class Application
{
public:
    int run()
    {
        aft::ThreadPool thread_pool;
        thread_pool.start(1);
        std::string command;
        bool stopeable = false;
        bool restartable = false;
        size_t current_size = 1;

        std::future<std::future<int64_t>> future;
        while(true)
        {
            std::cout << "Input command" << std::endl;
            std::getline( std::cin, command);
            std::cout << "Command " + command << std::endl;
            if (command == "exit"){
                stopeable = true;
                restartable = false;
            } else if (command == "restart"){
                stopeable = true;
                restartable = true;
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
            if ( restartable ) {
                thread_pool.start(current_size*2);
                stopeable = false;
                restartable = false;
            } else if (stopeable){
                break;
            }
        }
        return 0;
    }
};

int main()
{
    Application app;
    return app.run();
}
