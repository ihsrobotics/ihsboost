#include <ihsboost/message_buf.hpp>
#include <iostream>
#include <fstream>
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

    MessageBuf m2;
    m2.set_val<int>(33);

    cout << "getting val " << m2.get_val<int>() << endl;
    cout << "failing to get val" << endl;
    try
    {
        cout << m2.get_val<double>() << endl;
    }
    catch (BadBufCastException &exc)
    {
        cout << "failed with " << exc.what() << endl;
    }

    double my_arr[3] = {2, 3.14, 33.7};
    m.set_val<double, 3>(my_arr);
    double *ret = m.get_val<double, 3>();
    for (int i = 0; i < 3; ++i)
    {
        cout << "val at " << i << " is " << ret[i] << endl;
    }

    ofstream out("out.b", ofstream::binary);
    char *write_buf = m.to_bytes();
    cout << "difference between the two is " << MessageBuf::get_size<double, 3>() << " and " << m.get_size() << endl;
    out.write(reinterpret_cast<const char *>(write_buf), MessageBuf::get_size<double, 3>());
    out.close();
    delete[] write_buf;

    ifstream in("out.b", ofstream::binary);
    char *buf = new char[MessageBuf::get_size<double, 3>()];
    in.read(buf, m.get_size());

    cout << "recomposing" << endl;
    MessageBuf recomposed;
    recomposed.from_bytes(buf);
    double *second_ret = recomposed.get_val<double, 3>();
    for (int i = 0; i < 3; ++i)
    {
        cout << "val at " << i << " is " << second_ret[i] << endl;
    }

    cout << "making sure it retained type info " << endl;
    try
    {
        cout << recomposed.get_val<int>() << endl;
    }
    catch (BadBufCastException &exc)
    {
        cout << "retained type info! and got good error message: " << exc.what() << endl;
    }

    cout << "does it work getting just one value??" << endl;
    try
    {
        cout << recomposed.get_val<double>() << endl;
    }
    catch (BadBufCastException &exc)
    {
        cout << "caught the difference between arrays and single values!" << endl;
    }

    return 0;
}