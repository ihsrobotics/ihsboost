#include <iostream>
#include <chrono>
#include <thread>
#include "threading.hpp"

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

int main()
{
    cout << "starting" << endl;
    Threadable my_thread(cool_function, 3, 11, 2300);
    Threadable my_other_thread(cool_other_function, 3, 11, 22, 500);
    size_t i = 0;
    cout << "what is the current value? " << my_thread() << " and " << my_other_thread() << endl;
    while (!my_thread() || !my_other_thread())
    {
        ++i;
    }
    cout << "finished with i value of " << i << endl;
    return 0;
}