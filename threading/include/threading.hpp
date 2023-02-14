#ifndef THREADING_HPP
#define THREADING_HPP

#include <thread>

/**
 * @brief A thread that runs the given function with the given arguments
 *
 * @tparam Callable - the function
 * @tparam Args - the arguments
 */
template <typename Callable, typename... Args>
class Threadable
{
public:
    /**
     * @brief Construct a new Threadable object to run the given function
     * with the given parameters in a separate thread
     *
     * @param func the function to run
     * @param args the arguments to pass to the function
     */
    Threadable(Callable &func, Args... args) : done_flag(false), _func(&func), _thread(wrapper, this, args...){};

    /**
     * @brief Destroy the Threadable object
     * @details blocks execution while waiting for the thread to finish
     *
     */
    ~Threadable()
    {
        _thread.join();
    }

    /**
     * @brief Check the status of the thread
     *
     * @return true if the thread has completed
     * @return false if the thread has not yet completed
     */
    bool operator()() const { return done_flag; }

    /**
     * @brief Wait for the thread to finish.
     * @details This is a blocking function
     *
     */
    void join() { _thread.join(); }

private:
    /**
     * @brief Wrapper function to allow the use of threads with member functions
     *
     * @param threadable the Threadable object
     * @param args the arguments to pass to the Threadable object's function
     */
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