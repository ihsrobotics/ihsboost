#include "util.hpp"
#include "test.hpp"
#include <iostream>
using namespace std;

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

void test_save_config()
{
    // create a config
    int retval = system("echo '{ \"linear_accel\" : 20000.0 }' > cool.json");
    assert_equals(retval, 0, "creating extra config");

    // make sure it works regularly
    Config conf("cool.json");
    assert_equals(20000.0, conf.getDouble("linear_accel"), "getting actual config values in save_config");
    assert_equals(0, conf.getInt("other_key"));

    // set values
    conf.setDouble("linear_accel", 123.45);
    conf.setInt("other_key", 27);
    conf.save("cool.json");

    // reload config and see if it worked
    Config new_conf("cool.json");
    assert_equals(123.45, new_conf.getDouble("linear_accel"));
    assert_equals(27, new_conf.getInt("other_key"));

    // cleanup
    retval = system("rm cool.json");
    assert_equals(retval, 0, "removing cool json file");
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
    test_save_config();
    test_config();
    return 0;
}