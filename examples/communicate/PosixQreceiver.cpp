#include <ihsboost/all.hpp>
#include <iostream>

using namespace std;

class MyDummyClass
{
public:
    MyDummyClass(int a, int b) : value_a(a), value_b(b){};
    int get_value_a() { return value_a; }
    int get_value_b() { return value_b; }
    int get_sum() { return value_a + value_b; }

private:
    int value_a;
    int value_b;
};

int main()
{
    // create a posix q. THe name of the posix q must be similar to this (start with / and be a legitimate file name)
    PosixQCommunicator c("/my_posix_queue");

    // get an int from the communicator. The type in the brackets (int) determines what type
    // we are trying to get
    int first_val = c.receive_msg().get_val<int>();
    cout << "the int we received was " << first_val << endl;

    // receive a double from the communicator since the type in the brackets is double
    double second_val = c.receive_msg().get_val<double>();
    cout << "the double we received was " << second_val << endl;

    // receive a string as char*
    string a(c.receive_msg().get_ptr_val<char>());
    cout << "the string we received was " << a << endl;

    // we can also send other datatypes over communicators.
    // the only restriction is that they CANNOT contain pointers (since we can't guarantee that the receiving
    // side will be on the same process and thus probably won't be able to access those pointers)
    MyDummyClass cool_class = c.receive_msg().get_val<MyDummyClass>();
    cout << "our class has sum of " << cool_class.get_sum() << endl;

    return 0;
}