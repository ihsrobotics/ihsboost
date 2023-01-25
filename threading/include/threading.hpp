#ifndef THREADING_HPP
#define THREADING_HPP

#include <thread>

template <typename Callable, typename... Args>
class Threadable
{
public:
    Threadable(Callable &func, Args... args) : done_flag(false), _func(&func), _thread(wrapper, this, args...){};
    ~Threadable()
    {
        _thread.join();
    }
    bool operator()() const { return done_flag; }
    void join() { _thread.join(); }

private:
    static void wrapper(Threadable<Callable, Args...> *threadable, Args... args)
    {
        threadable->_func(args...);
        threadable->done_flag = true;
    }

    Callable *_func;
    volatile bool done_flag;
    std::thread _thread;
};

#endif