#include <iostream>
#include <thread_pool.h>
#include <string>
#include <vector>

namespace aft = abstract::from::thread;

class ISubmiter
{
public:
    virtual bool prepare(const std::string& description) = 0;
    virtual void submit() = 0;
};

class ThreadPoolSubmiter: public ISubmiter
{
public:
    ThreadPoolSubmiter(std::weak_ptr<aft::ThreadPool> thread_pool_sptr)
        :
    {

    }
private:
    std::shared_ptr<aft::ThreadPool>
};

class SimpleDelimeter: public ISubmiter
{
public:
    bool prepare(const std::string& description)
    {

    }
};

class SubmitersFactory
{
public:
    bool registrate();
private:
};

void initEnvironment()
{

}

int main()
{
    return 0;
}
