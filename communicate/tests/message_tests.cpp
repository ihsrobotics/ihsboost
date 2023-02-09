#include <ihsboost/message_buf.hpp>
#include <iostream>
using namespace std;

int main()
{
    MessageBuf m;
    m.set_val<int>(223);
    cout << "getting value is " << m.get_val<int>() << endl;

    cout << "testing if it stores type info" << endl;
    try
    {
        cout << m.get_val<string>() << endl;
    }
    catch (BadBufCastException &b)
    {
        cout << "bad buf cast with exception: " << b.what() << endl;
    }

    MessageBuf c;
    c.from_bytes(m.to_bytes());
    cout << "testing if it retains information" << endl;
    try
    {
        cout << c.get_val<string>() << endl;
    }
    catch (BadBufCastException &b)
    {
        cout << "bad buf cast with exception: " << b.what() << endl;
    }
    cout << "it was actually " << c.get_val<int>() << endl;

    cout << "testing if we can change our value without changing the previous one" << endl;
    c.set_val<string>("hi there cutie");
    cout << "c is now " << c.get_val<string>() << endl;
    cout << "m is still " << m.get_val<int>() << endl;

    return 0;
}