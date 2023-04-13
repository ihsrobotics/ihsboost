/**
 * @file json_loader.cpp
 * @author Eliot Hall
 * @brief File that demonstrates example loading of config values
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>
#include <ihsboost/util/json_config.hpp>
#include "json_loader.hpp"
using namespace std;

int main()
{
    cout << "we successfully loaded configs from bot-config.json and other-config.json" << endl;
    cout << "values in bot-config.json:" << endl;
    cout << "arm position : " << arm_position << ", owner: " << owner << endl;
    cout << "values in other-config.json:" << endl;
    cout << "a: " << a << ", b: " << b << ", c: " << c << ", is_good: " << is_good << ", is_bad: " << is_bad << endl;
    return 0;
}