#include "threadable.hpp"

Threadable::Threadable()
    : _started(false), _done(false), _thread(), _func(nullptr){};
Threadable::~Threadable() {
    // stop thread
    join();

    // delete function storage
    if (_func != nullptr) {
        delete _func;
    }
}
Threadable &Threadable::operator=(Threadable &&other) {
    if (this == &other) {
        return *this;
    }

    // move or copy the values
    _thread = std::move(other._thread);
    _func = std::move(other._func);
    _done = other._done;
    _started = other._started;

    // steal other's _func and make sure it doesn't delete it
    other._func = nullptr;
    return *this;
}

void Threadable::start() {
    if (!_started) {
        _thread = std::thread(&Threadable::wrapper, this);
        _done = false;
        _started = true;
    }
}

// modifiers
void Threadable::join() {
    if (_thread.joinable()) {
        _thread.join();
    }
}

// getters
bool Threadable::done() const { return _done; }
bool Threadable::operator()() const { return done(); }
bool Threadable::started() const { return _started; }

void Threadable::wrapper() {
    // call function if it exists
    if (_func != nullptr) {
        _func->call();
    }

    // cleanup variables
    _done = true;
    _started = false;
}