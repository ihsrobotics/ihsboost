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
#include <tuple>
#include <type_traits>

/**
 * @brief A thread that runs the given function with the given arguments
 *
 */
class Threadable {
  public:
    /**
     * @brief Construct a new Threadable object that does nothing
     *
     */
    Threadable();

    /**
     * @brief Construct a new Threadable object to run the given member function
     * on the given instance with the given parameters in a separate thread
     * @details upon creation, a Threadable is considered not done and not
     * started
     *
     * @tparam _MemberFunc the type of the member function to call
     * @tparam _Class the type the instance
     * @tparam _Args the types of the arguments to pass to the thread
     * @tparam
     * std::enable_if<std::is_member_function_pointer<_MemberFunc>::value,
     * bool>::type used to enforce template specialization
     * @param func the member function to call. In most circumstances, this is
     * `&CLASS_NAME::METHOD_NAME` where CLASS_NAME is the name of the class and
     * METHOD_NAME is the name of the method
     * @param c a pointer to the instance from which to run the member function.
     * @param args the arguments with which to call the member function. Note
     * that these can be lvalues or rvalues
     */
    template <typename _MemberFunc,
              typename _Class,
              typename... _Args,
              typename std::enable_if<
                  std::is_member_function_pointer<_MemberFunc>::value,
                  bool>::type = true>
    Threadable(_MemberFunc &&func, _Class *c, _Args &&...args)
        : _started(false),
          _done(false),
          _thread(),
          _func(
              new MemberFunctionWrapper<typename std::decay<_MemberFunc>::type,
                                        typename std::decay<_Class>::type,
                                        _Args...>(func, c, args...)){};

    /**
     * @brief Construct a new Threadable object to run the given function
     * with the given parameters in a separate thread
     * @details upon creation, a Threadable is considered not done and not
     * started.
     *
     * @tparam _Callable the type of the function to call
     * @tparam _Args the types of the arguments to pass to the thread
     * @param func the function to run
     * @param args the arguments to pass to the function. Note that these can be
     * lvalues or rvalues
     */
    template <typename _Callable, typename... _Args>
    Threadable(_Callable &&func, _Args &&...args)
        : _started(false),
          _done(false),
          _thread(),
          _func(new StaticFunctionWrapper<typename std::decay<_Callable>::type,
                                          _Args...>(func, args...)){};

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
    /**
     * @brief Equals operator for setting a Threadable equal to
     * a Threadable rvalue
     *
     * @param other the Threadable to set this equal to
     * @return Threadable& this Threadable
     */
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
    class FunctionWrapper {
      public:
        /**
         * @brief Call the function with any associated arguments
         *
         */
        virtual void call() = 0;
        virtual ~FunctionWrapper(){};

      protected:
        template <std::size_t... Ts> struct index {};

        template <std::size_t N, std::size_t... Ts>
        struct gen_seq : gen_seq<N - 1, N - 1, Ts...> {};

        /**
         * @brief Generate a sequence of indexes given the size of
         * a parameter pack
         *
         * @tparam Ts
         */
        template <std::size_t... Ts> struct gen_seq<0, Ts...> : index<Ts...> {};
    };

    /**
     * @brief Function wrapper class for static functions (functions
     * that aren't member functions)
     *
     * @tparam _StaticFunc the type of the static function to call
     * @tparam _Args the types of the arguments that will be passed
     */
    template <typename _StaticFunc, typename... _Args>
    class StaticFunctionWrapper : public FunctionWrapper {
      private:
        std::tuple<_Args...>
            _args;         ///< used for storing all the arguments in a tuple
        _StaticFunc _func; ///< the static function to call

        /**
         * @brief Unpack the tuple by getting all the arguments by index
         *
         * @tparam Is all the indexes
         */
        template <std::size_t... Is>
        void func_caller(FunctionWrapper::index<Is...>) {
            _func(std::get<Is>(_args)...);
        }

      public:
        /**
         * @brief Construct a new Static Function Wrapper object
         *
         * @param func the function to call
         * @param args the arguments to call the function with
         */
        StaticFunctionWrapper(_StaticFunc func, _Args... args)
            : _args(std::forward<_Args>(args)...), _func(func) {}
        virtual ~StaticFunctionWrapper() = default;
        virtual void call() { func_caller(gen_seq<sizeof...(_Args)>{}); }
    };

    /**
     * @brief Function wrapper class for member functions
     *
     * @tparam _MemberFunc the type of the member function to call
     * @tparam _Class the type of the class that will call it
     * @tparam _Args the types of the arguments that will be passed
     */
    template <typename _MemberFunc, typename _Class, typename... _Args>
    class MemberFunctionWrapper : public FunctionWrapper {
      private:
        std::tuple<_Args...> _args; ///< used for storing arguments in a tuple
        _Class *_ptr; ///< pointer to instance that calls the member function
        _MemberFunc _func; ///< pointer to the member function to call

        /**
         * @brief Unpack the tuple by getting all the arguments by index
         *
         * @tparam Is all the indexes
         */
        template <std::size_t... Is>
        void func_caller(FunctionWrapper::index<Is...>) {
            (_ptr->*_func)(std::get<Is>(_args)...);
        }

      public:
        /**
         * @brief Construct a new Member Function Wrapper object
         *
         * @param func the function to call
         * @param ptr the instance to call the function from
         * @param args the arguments to call the function with
         */
        MemberFunctionWrapper(_MemberFunc func, _Class *ptr, _Args... args)
            : _args(std::forward<_Args>(args)...), _ptr(ptr), _func(func) {}
        virtual ~MemberFunctionWrapper() = default;
        virtual void call() { func_caller(gen_seq<sizeof...(_Args)>{}); }
    };

    bool _started;          ///< whether or not the thread was started
    volatile bool _done;    ///< whether or not the thread is done
    std::thread _thread;    ///< the thread itself
    FunctionWrapper *_func; ///< the function to call
};

#endif
/**@}*/