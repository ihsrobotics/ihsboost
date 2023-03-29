#include "util.hpp"
#include "test.hpp"
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace chrono;

void test_get_config()
{
    // test to make sure getting config and setting extra config works properly
    // create a config
    int retval = system("echo '{ \"linear_accel\" : 20000.0 }' > cool.json");
    assert_equals(retval, 0, "creating extra config");

    // check whether setting it works
    set_extra_config("cool.json");
    assert_equals(get_config().getDouble("linear_accel"), 20000.0, "getting overriden value");
    assert_equals(get_config().getInt("align_updates_per_sec"), 500, "getting default value");

    // cleanup
    retval = system("rm cool.json");
    assert_equals(retval, 0, "removing extra config");
}

void test_config()
{
    // create a config
    int retval = system("echo '{ \"val\" : 33 }' > bot-config.json");
    assert_equals(retval, 0, "creating bot-config json file");

    // test whether getting actual and non-existent values works
    Config c;
    assert_equals(c.getInt("val"), 33, "getting actual config value");
    assert_equals(c.getInt("non_existent"), 0, "getting non-existent config value");

    // cleanup
    retval = system("rm bot-config.json");
    assert_equals(retval, 0, "removing bot-config json file");
}

int main()
{
    test_get_config();
    test_config();
    return 0;
}