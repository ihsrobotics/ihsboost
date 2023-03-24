/**
 * @file threadable.hpp
 * @author Eliot Hall
 * @brief Easy threading functionality
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup threading_id
 * @{
 */
#ifndef IHSBOOST_THREADABLE_HPP
#define IHSBOOST_THREADABLE_HPP

#include <thread>
#include <functional>

/**
 * @brief A thread that runs the given function with the given arguments
 *
 */
class Threadable
{
public:
    /**
     * @brief Construct a new Threadable object to run the given function
     * with the given parameters in a separate thread
     * @details upon creation, a Threadable is considered not done and not started.
     *
     * @param func the function to run
     * @param args the arguments to pass to the function
     */
    template <typename _Callable, typename... _Args>
    Threadable(_Callable &&func, _Args... args) : _started(false), _done(false),
                                                  _thread(), _func([func, args...]() -> void
                                                                   { func(args...); }){};

    /**
     * @brief Destroy the Threadable object
     * @details blocks execution while waiting for the thread to finish
     *
     */
    ~Threadable();

    /**
     * @brief Start the thread
     * @details After calling this, the thread is considered
     * started and not considered done until it has finished
     * executing.
     *
     */
    void start();

    /**
     * @brief Wait for the thread to finish.
     * @details This is a blocking function
     *
     */
    void join();

    /**
     * @brief Return whether or not the thread has completed
     *
     * @return true - it has completed
     * @return false - it hasn't completed yet OR it hasn't been started yet
     */
    bool done() const;

    /**
     * @brief Return whether or not the thread was started
     * @details Once the thread has finished, it is considered
     * not started again.
     *
     * @return true
     * @return false
     */
    bool started() const;

private:
    /**
     * @brief Wrapper function to allow the use of threads with member functions
     *
     * @param threadable the Threadable object
     * @param args the arguments to pass to the Threadable object's function
     */
    static void wrapper(Threadable *threadable);

    bool _started;               ///< whether or not the thread was started
    volatile bool _done;         ///< whether or not the thread is done
    std::thread _thread;         ///< the thread itself
    std::function<void()> _func; ///< the function to call
};

#endif
/**@}*/