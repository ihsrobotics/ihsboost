#include <ihsboost/message_buf.hpp>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    int max_msg_size = 100;
    MessageBuf m(max_msg_size);
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

    MessageBuf c(max_msg_size);
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
    string our_cool_msg = "hi there cutie";
    c.set_val<char>(our_cool_msg.c_str(), our_cool_msg.size());
    cout << "c is now " << c.get_ptr_val<char>() << endl;
    cout << "m is still " << m.get_val<int>() << endl;

    MessageBuf m2(max_msg_size);
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
    m.set_val<double>(my_arr, 3);
    double *ret = m.get_ptr_val<double>();
    for (int i = 0; i < 3; ++i)
    {
        cout << "val at " << i << " is " << ret[i] << endl;
    }

    ofstream out("out.b", ofstream::binary);
    char *write_buf = m.to_bytes();
    cout << "difference between the two is " << MessageBuf::get_size(max_msg_size) << " and " << m.get_true_size() << " and " << m.get_buffered_size() << endl;

    out.write(reinterpret_cast<const char *>(write_buf), MessageBuf::get_size(max_msg_size));
    out.close();
    delete[] write_buf;

    ifstream in("out.b", ofstream::binary);
    char *buf = new char[MessageBuf::get_size(max_msg_size)];
    in.read(buf, MessageBuf::get_size(max_msg_size));

    cout << "recomposing" << endl;
    MessageBuf recomposed(max_msg_size);
    recomposed.from_bytes(buf);
    double *second_ret = recomposed.get_ptr_val<double>();
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