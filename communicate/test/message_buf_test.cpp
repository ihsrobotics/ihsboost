#include "test.hpp"
#include "message_buf.hpp"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void test_get_val_int(int max_msg_size, int val)
{
    MessageBuf m(max_msg_size);
    m.set_val<int>(val);
    assert_equals(val, m.get_val<int>(), "checking get val int");
}
void test_get_val_double(int max_msg_size, double val)
{
    MessageBuf m(max_msg_size);
    m.set_val<double>(val);
    assert_equals(val, m.get_val<double>(), "checking get val double");
}
void test_get_val_string(int max_msg_size, string val)
{
    MessageBuf m(max_msg_size);
    m.set_val<char>(val.c_str(), static_cast<uint16_t>(val.size()) + 1);
    assert_equals(val, string(m.get_ptr_val<char>()), "checking get val string");
}

void test_type_info(int max_msg_size)
{
    MessageBuf m(max_msg_size);
    bool success = true;

    // try ints
    m.set_val<int>(100);
    try
    {
        m.get_val<string>();
        success &= false;
    }
    catch (BadBufCastException &b)
    {
    };
    try
    {
        m.get_val<double>();
        success &= false;
    }
    catch (BadBufCastException &b)
    {
    };

    // try double
    m.set_val<double>(127.34);
    try
    {
        m.get_val<string>();
        success &= false;
    }
    catch (BadBufCastException &b)
    {
    };
    try
    {
        m.get_val<int>();
        success &= false;
    }
    catch (BadBufCastException &b)
    {
    };

    // try strings
    string str_msg = "hello world";
    m.set_val<char>(str_msg.c_str(), static_cast<uint16_t>(str_msg.size()) + 1);
    try
    {
        m.get_val<int>();
        success &= false;
    }
    catch (BadBufCastException &b)
    {
    };
    try
    {
        m.get_val<double>();
        success &= false;
    }
    catch (BadBufCastException &b)
    {
    };

    assert_equals(true, success, "retaining type info");
}

void test_recompose(int max_msg_size)
{
    MessageBuf m(max_msg_size);
    MessageBuf c(max_msg_size);
    int val = 33;
    m.set_val<int>(val);

    // make sure recomposing works
    c.from_bytes(m.to_bytes());
    assert_equals(m.get_val<int>(), c.get_val<int>(), "getting recomposed value");

    // make sure we can change the value
    string alt_val = "hi there";
    c.set_val<char>(alt_val.c_str(), static_cast<uint16_t>(alt_val.size()) + 1);

    // make sure the original value is unchanged
    assert_equals(val, m.get_val<int>(), "checking for unchanged original value");
}

void test_int_double_ptrs(int max_msg_size)
{
    MessageBuf m(max_msg_size);

    // test double vals
    double my_arr[3] = {2, 3.14, 33.7};
    m.set_val<double>(my_arr, 3);
    double *ret = m.get_ptr_val<double>();
    for (int i = 0; i < 3; ++i)
    {
        assert_equals(my_arr[i], ret[i], "getting double val");
    }

    // test int vals
    int my_int_arr[7] = {3, 1, 5, 8, 23, 6, 12};
    m.set_val<int>(my_int_arr, 7);
    int *int_ret = m.get_ptr_val<int>();
    for (int i = 0; i < 7; ++i)
    {
        assert_equals(my_int_arr[i], int_ret[i], "getting int val");
    }
}

void test_file_recomposition(int max_msg_size)
{
    MessageBuf m(max_msg_size);

    // make vals
    vector<int> vals;
    for (int i = 0; i < 10; ++i)
    {
        vals.push_back(rand() % 100);
    }

    // store vals in the buffer
    m.set_val<int>(vals.begin().base(), static_cast<uint16_t>(vals.size()));

    // make the ofstream and write to it
    ofstream out("out.b", ofstream::binary);
    char *write_buf = m.to_bytes();
    out.write(reinterpret_cast<const char *>(write_buf), MessageBuf::get_size(max_msg_size));
    out.close();
    delete[] write_buf;

    // open it and recompose
    ifstream in("out.b", ofstream::binary);
    char *buf = new char[MessageBuf::get_size(max_msg_size)];
    in.read(buf, MessageBuf::get_size(max_msg_size));
    MessageBuf recomposed(max_msg_size);
    recomposed.from_bytes(buf);

    // make sure it is the same as the original
    for (size_t i = 0; i < vals.size(); ++i)
    {
        assert_equals(vals[i], *(recomposed.get_ptr_val<int>() + i), "getting recomposed values");
    }

    // make sure it retains type information
    bool success = true;
    try
    {
        cout << recomposed.get_ptr_val<double>() << endl;
        success &= false;
    }
    catch (BadBufCastException &exc)
    {
    }

    // make sure it retains pointer/not pointer information
    try
    {
        cout << recomposed.get_val<int>() << endl;
        success &= false;
    }
    catch (BadBufCastException &exc)
    {
    }
    assert_equals(true, success, "retaining information after recomposition");

    // cleanup
    assert_equals(0, system("rm ./out.b"), "cleaning up");
}

int main()
{
    int max_msg_size = 100;

    // make sure get val is working
    test_get_val_int(max_msg_size, 223);
    test_get_val_int(max_msg_size, -2394);
    test_get_val_int(max_msg_size, 514);
    test_get_val_double(max_msg_size, 3.14);
    test_get_val_double(max_msg_size, 1234.56);
    test_get_val_double(max_msg_size, 17.326);
    test_get_val_string(max_msg_size, "hello world");
    test_get_val_string(max_msg_size, "the sly brown fox jumped over the");
    test_get_val_string(max_msg_size, "goodbye");

    // make sure type info is retained
    test_type_info(max_msg_size);

    // make sure recomposition works
    test_recompose(max_msg_size);

    // make sure ptr vals work for ints and doubles
    test_int_double_ptrs(max_msg_size);

    // make sure the file works
    test_file_recomposition(max_msg_size);

    return 0;
}