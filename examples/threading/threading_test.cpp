#include <iostream>
#include <chrono>
#include <thread>
#include <ihsboost/threading.hpp>

using namespace std;
using namespace chrono;

void cool_function(int a, int b, int sleep)
{
    cout << "going to add them eventually" << endl;
    this_thread::sleep_for(milliseconds(sleep));
    cout << "a + b is " << a + b << endl;
}

void cool_other_function(int a, int b, int c, int sleep)
{
    cout << "going to multiply eventually" << endl;
    this_thread::sleep_for(milliseconds(sleep));
    cout << "a * b * c is " << a * b * c << endl;
}

void lazy_function()
{
    cout << "lazing" << endl;
    this_thread::sleep_for(milliseconds(500));
    cout << "done lazing" << endl;
}

int main()
{
    cout << "starting" << endl;

    // In order to create a threadable, it's just like creating
    // a std::thread; just pass the function and any arguments
    Threadable my_thread(cool_function, 3, 11, 2300);
    Threadable my_other_thread(cool_other_function, 3, 11, 22, 500);
    Threadable lazy(lazy_function); // you can also do this with a 0 parameter function
    my_thread.start();
    my_other_thread.start();
    lazy.start();
    size_t i = 0;
    cout << "what is the current value? " << my_thread.done() << " and " << my_other_thread.done() << endl;
    while (!my_thread.done() || !my_other_thread.done() || !lazy.done()) // keep going until they're all done
    {
        ++i;
    }
    cout << "finished with i value of " << i << endl;
    return 0;
}