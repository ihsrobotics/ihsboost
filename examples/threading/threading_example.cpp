/**
 * @file threading_example.cpp
 * @author Eliot Hall
 * @brief File that demonstrates how to use Threadable
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <chrono>
#include <ihsboost/threading/threading.hpp>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;

void cool_function(int a, int b, int sleep) {
    cout << "going to add them eventually" << endl;
    this_thread::sleep_for(milliseconds(sleep));
    cout << "a + b is " << a + b << endl;
}

void cool_other_function(int a, int b, int c, int sleep) {
    cout << "going to multiply eventually" << endl;
    this_thread::sleep_for(milliseconds(sleep));
    cout << "a * b * c is " << a * b * c << endl;
}

void lazy_function() {
    cout << "lazing" << endl;
    this_thread::sleep_for(milliseconds(500));
    cout << "done lazing" << endl;
}

class DummyClass {
  public:
    DummyClass() : num(0){};
    int get_num() { return num; }
    void add_num(int ms, int modifier) {
        time_point<system_clock> start = system_clock::now();
        time_point<system_clock> now = system_clock::now();
        while (duration_cast<milliseconds>(now - start).count() < ms) {
            num += modifier;
            this_thread::sleep_for(milliseconds(10));
            now = system_clock::now();
        }
    }

  private:
    int num;
};

int main() {
    cout << "starting" << endl;

    // In order to create a threadable, it's just like creating
    // a std::thread; just pass the function and any arguments
    Threadable my_thread(cool_function, 3, 11, 2300);
    Threadable my_other_thread(cool_other_function, 3, 11, 22, 500);
    Threadable lazy(
        lazy_function); // you can also do this with a 0 parameter function

    // start the threads; they don't start by default
    my_thread.start();
    my_other_thread.start();
    lazy.start();

    // wait till they finish
    size_t i = 0;
    cout << "what is the current value? " << my_thread.done() << " and "
         << my_other_thread.done() << endl;
    while (!my_thread.done() || !my_other_thread.done() ||
           !lazy.done()) // keep going until they're all done
    {
        ++i;
    }
    cout << "finished with i value of " << i << endl;

    // threads can modify objects too!
    // syntax: &Class::function, &instance, arguments
    DummyClass dummy_class;
    Threadable modify_by_100(
        &DummyClass::add_num,
        &dummy_class,
        500,
        100); // call add_num with ms time of 500 and update of 10
    Threadable modify_by_10(&DummyClass::add_num, &dummy_class, 2000, 10);

    modify_by_10.start();
    modify_by_100.start();
    while (!modify_by_10.done() || !modify_by_100.done()) {
        cout << "value: " << dummy_class.get_num() << "\r";
    }
    cout << endl;
    return 0;
}