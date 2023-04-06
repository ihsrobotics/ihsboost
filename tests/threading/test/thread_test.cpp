#include "test.hpp"
#include "threading.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <numeric>
#include <mutex>
#include <vector>

using namespace std;
using namespace chrono;

class Test
{
public:
    Test(int val) : val(val){};
    void increment_val(int increment_amt) { val += increment_amt; }
    int get_val() { return val; }

private:
    mutex m;
    int val;
};

void a(Test &t, int amt)
{
    this_thread::sleep_for(milliseconds(200));
    cout << "adding a with " << amt << endl;
    t.increment_val(amt);
    this_thread::sleep_for(milliseconds(100));
}

void test_single_thread_ptr()
{
    Test test(0);

    int amt = 10;
    Threadable *thread = nullptr;

    // create threadable
    thread = new Threadable(a, test, amt);

    // run threadable
    thread->start();

    // wait for threads to complete
    while (!thread->done())
        ;

    // check sum
    assert_equals(amt, test.get_val(), "checking pointer sum");
    delete thread;
    cout << "passed one val ptr" << endl;
}
void test_single_thread()
{
    Test test(0);

    int amt = 10;
    Threadable thread;

    // create threadable
    thread = Threadable(a, test, amt);

    // run threadable
    thread.start();

    // wait for threads to complete
    while (!thread.done())
        ;

    // check sum
    assert_equals(amt, test.get_val(), "checking sum");
    cout << "passed one thread" << endl;
}

void test_two_threads_ptr()
{
    Test test(0);

    int amt1 = 10;
    int amt2 = 30;
    Threadable *thread1 = nullptr;
    Threadable *thread2 = nullptr;

    // create threadable
    thread1 = new Threadable(a, test, amt1);
    thread2 = new Threadable(a, test, amt2);

    // run threadable
    thread1->start();
    thread2->start();

    // wait for threads to complete
    while (!thread1->done() || !thread2->done())
        ;

    // check sum
    assert_equals(amt1 + amt2, test.get_val(), "checking 2 pointer sum");

    // cleanup
    delete thread1;
    delete thread2;

    cout << "passed two threads ptr" << endl;
}
void test_two_threads()
{
    Test test(0);

    int amt1 = 10;
    int amt2 = 30;
    Threadable thread1;
    Threadable thread2;

    // create threadable
    thread1 = Threadable(a, test, amt1);
    thread2 = Threadable(a, test, amt2);

    // run threadable
    thread1.start();
    thread2.start();

    // wait for threads to complete
    while (!thread1.done() || !thread2.done())
        ;

    // check sum
    assert_equals(amt1 + amt2, test.get_val(), "checking 2 sum");
    cout << "passed two threads" << endl;
}

void test_multiple_threads_ptr()
{
    Test test(0);

    int num_threads = 5;
    vector<int> amts = {10, 20, 30, 40, 50};
    vector<Threadable *> threadables(num_threads);

    // create threadables
    for (int i = 0; i < num_threads; ++i)
    {
        threadables[i] = new Threadable(a, test, amts[i]);
    }

    // run threads
    for (int i = 0; i < num_threads; ++i)
    {
        threadables[i]->start();
    }

    // wait for done
    bool done = false;
    while (!done)
    {
        done = true;
        for (int i = 0; i < num_threads; ++i)
        {
            done &= threadables[i]->done();
        }
    }

    // check sum
    assert_equals(accumulate(amts.begin(), amts.end(), 0), test.get_val(), "checking multiple sum");

    // cleanup
    for (int i = 0; i < num_threads; ++i)
    {
        delete threadables[i];
        threadables[i] = nullptr;
    }
    cout << "passed multiple threads ptr" << endl;
}
void test_multiple_threads()
{
    Test test(0);

    int num_threads = 5;
    vector<int> amts = {10, 20, 30, 40, 50};
    vector<Threadable> threadables(num_threads);

    // create threadables
    for (int i = 0; i < num_threads; ++i)
    {
        threadables[i] = Threadable(a, test, amts[i]);
    }

    // run threads
    for (int i = 0; i < num_threads; ++i)
    {
        threadables[i].start();
    }

    // wait for done
    bool done = false;
    while (!done)
    {
        done = true;
        for (int i = 0; i < num_threads; ++i)
        {
            done &= threadables[i].done();
        }
    }

    assert_equals(accumulate(amts.begin(), amts.end(), 0), test.get_val(), "checking multiple sum");
    cout << "passed multiple threads" << endl;
}

void test_dynamic(int val1, int val2)
{
    Test test(0);

    Threadable t1(a, test, val1);
    Threadable t2(a, test, val2);

    t1.start();
    t2.start();

    while (!t1.done() || !t2.done())
        ;

    assert_equals(val1 + val2, test.get_val(), "testing dynamic");
    cout << "passed test dynamic" << endl;
}

void test_member_func()
{
    Test test(0);
    int amt1 = 10;
    int amt2 = 20;
    Threadable t1(&Test::increment_val, &test, amt1);
    Threadable t2(&Test::increment_val, &test, amt2);

    t1.start();
    t2.start();

    while (!t1.done() || !t2.done())
        ;

    assert_equals(amt1 + amt2, test.get_val(), "testing members");
    cout << "passed member funcs" << endl;
}

int main()
{
    test_single_thread_ptr();
    test_two_threads_ptr();

    test_single_thread();
    test_two_threads();

    test_multiple_threads_ptr();
    test_multiple_threads();

    test_dynamic(rand() % 10, rand() % 50);
    test_member_func();

    return 0;
}