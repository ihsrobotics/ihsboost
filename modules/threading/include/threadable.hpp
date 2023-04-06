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
#include <tuple>

/**
 * @brief A thread that runs the given function with the given arguments
 *
 */
class Threadable
{
public:
    /**
     * @brief Construct a new Threadable object that does nothing
     *
     */
    Threadable();

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
     * @param args the arguments with which to call the member function. Note that these should be lvalues
     */
    template <typename _MemberFunc, typename _Class, typename... _Args,
              typename std::enable_if<std::is_member_function_pointer<_MemberFunc>::value, bool>::type = true>
    Threadable(_MemberFunc &&func, _Class *c, _Args &&...args) : _started(false),
                                                                 _done(false),
                                                                 _thread(), _func(new MemberFunctionWrapper<_Class, _Args...>(func, c, args...)){};

    /**
     * @brief Construct a new Threadable object to run the given function
     * with the given parameters in a separate thread
     * @details upon creation, a Threadable is considered not done and not started.
     *
     * @param func the function to run
     * @param args the arguments to pass to the function
     */
    template <typename _Callable, typename... _Args>
    Threadable(_Callable &&func, _Args &&...args) : _started(false), _done(false),
                                                    _thread(), _func(new StaticFunctionWrapper<_Args...>(func, args...)){};

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

    Threadable &operator=(const Threadable &other) = delete;
    Threadable &operator=(Threadable &&other);

private:
    /**
     * @brief Wrapper function that sets done to true after finishing
     *
     */
    void wrapper();

    /**
     * @brief Function wrapper class to allow for runtime polymorphism
     *
     */
    class FunctionWrapper
    {
    public:
        /**
         * @brief Call the function with any associated arguments
         *
         */
        virtual void call() = 0;
        virtual ~FunctionWrapper(){};
    };

    /**
     * @brief Function wrapper class for static functions (functions
     * that aren't member functions)
     *
     * @tparam _Args the types of the arguments that will be passed
     */
    template <typename... _Args>
    class StaticFunctionWrapper : public FunctionWrapper
    {
    private:
        std::tuple<_Args...> _args;
        std::function<void(_Args...)> _func;

    public:
        /**
         * @brief Construct a new Static Function Wrapper object
         *
         * @tparam _StaticFunc type of the function; automatically deduced. The
         * reason this is here is to allow universal references because of
         * template deduction
         * @param func the function to call
         * @param args the arguments to call the function with
         */
        template <typename _StaticFunc>
        StaticFunctionWrapper(_StaticFunc &&func, _Args &&...args) : _args(std::forward<_Args>(args)...), _func(func) {}
        virtual ~StaticFunctionWrapper() = default;
        virtual void call() { _func(std::get<_Args>(_args)...); }
    };

    /**
     * @brief Function wrapper class for member functions
     *
     * @tparam _Class the type of the class that will call it
     * @tparam _Args the types of the arguments that will be passed
     */
    template <typename _Class, typename... _Args>
    class MemberFunctionWrapper : public FunctionWrapper
    {
    private:
        std::tuple<_Args...> _args;                    ///< used for storing arguments in a tuple
        _Class *_ptr;                                  ///< pointer to instance that calls it
        std::function<void(_Class *, _Args...)> _func; ///< function to call

    public:
        /**
         * @brief Construct a new Member Function Wrapper object
         *
         * @tparam _MemberFunc type of the function; automatically deduced. The
         * reason this is here is to allow universal references because of
         * template deduction
         * @param func the function to call
         * @param ptr the instance to call the function from
         * @param args the arguments to call the function with
         */
        template <typename _MemberFunc>
        MemberFunctionWrapper(_MemberFunc &&func, _Class *ptr, _Args &&...args) : _args(std::forward<_Args>(args)...), _ptr(ptr), _func(func) {}
        virtual ~MemberFunctionWrapper() = default;
        virtual void call() { _func(_ptr, std::get<_Args>(_args)...); }
    };

    bool _started;          ///< whether or not the thread was started
    volatile bool _done;    ///< whether or not the thread is done
    std::thread _thread;    ///< the thread itself
    FunctionWrapper *_func; ///< the function to call
};

#endif
/**@}*/