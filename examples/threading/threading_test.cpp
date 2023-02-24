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
    Threadable<void(int a, int b, int sleep), int, int, int> my_thread(cool_function, 3, 11, 2300);
    Threadable<void(int a, int b, int c, int sleep), int, int, int, int> my_other_thread(cool_other_function, 3, 11, 22, 500);
    Threadable<void()> lazy(lazy_function); // you can also do this with 0 parameters to your function!!!
    size_t i = 0;
    cout << "what is the current value? " << my_thread() << " and " << my_other_thread() << endl;
    while (!my_thread() || !my_other_thread() || !lazy())
    {
        ++i;
    }
    cout << "finished with i value of " << i << endl;
    return 0;
}