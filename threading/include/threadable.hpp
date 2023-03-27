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
     * @brief Construct a new Threadable object to run the given member function
     * on the given instance with the given parameters in a separate thread
     * @details upon creation, a Threadable is considered not done and not started
     *
     * @tparam _MemberFunc the type of the member function to call
     * @tparam _Class_Ptr the type, as a pointer, of the instance
     * @tparam _Args Types of the arguments to pass to the thread
     * @tparam std::enable_if<std::is_member_function_pointer<_MemberFunc>::value, bool>::type used to enforce template specialization
     * @tparam std::enable_if<std::is_pointer<_Class_Ptr>::value, bool>::type used to enforce template specialization
     * @param func the member function to call. In most circumstances, this is `&CLASS_NAME::METHOD_NAME`
     * where CLASS_NAME is the name of the class and METHOD_NAME is the name of the method
     * @param c a pointer to the instance from which to run the member function.
     * @param args the arguments with which to call the member function
     */
    template <typename _MemberFunc, typename _Class_Ptr, typename... _Args,
              typename std::enable_if<std::is_member_function_pointer<_MemberFunc>::value, bool>::type = true,
              typename std::enable_if<std::is_pointer<_Class_Ptr>::value, bool>::type = true>
    Threadable(_MemberFunc &&func, _Class_Ptr c, _Args &&...args) : _started(false),
                                                                    _done(false),
                                                                    _thread(), _func([func, c, args...]() -> void
                                                                                     { (c->*func)(args...); }){};

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
     * @details equivalent to calling done \see done
     *
     * @return true - it has completed
     * @return false - it hasn't completed yet OR it hasn't been started yet
     */
    bool operator()() const;

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
     * @return true - it has been started
     * @return false - it hasn't been started OR it has already completed
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