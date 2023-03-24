#include "threadable.hpp"

Threadable::~Threadable()
{
    _thread.join();
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
void Threadable::join() { _thread.join(); }

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