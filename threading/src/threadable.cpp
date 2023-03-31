#include "threadable.hpp"

Threadable::Threadable() : _started(false), _done(false), _thread(), _func(){};
Threadable::~Threadable() { join(); }
Threadable &Threadable::operator=(Threadable &&other)
{
    if (this == &other)
    {
        return *this;
    }

    // move or copy the values
    _thread = std::move(other._thread);
    _func = std::move(other._func);
    _done = other._done;
    _started = other._started;
    return *this;
}

void Threadable::start()
{
    if (!_started)
    {
        _thread = std::thread(wrapper, this);
        _done = false;
        _started = true;
    }
}

// modifiers
void Threadable::join()
{
    if (_thread.joinable())
    {
        _thread.join();
    }
}

// getters
bool Threadable::done() const { return _done; }
bool Threadable::operator()() const { return done(); }
bool Threadable::started() const { return _started; }

void Threadable::wrapper(Threadable *threadable)
{
    // call function
    threadable->_func();

    // cleanup variables
    threadable->_done = true;
    threadable->_started = false;
}