/**
 * @file configure_ihsboost.cpp
 * @author Eliot Hall
 * @brief File that demonstrates how to set ihsboost variables
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <ihsboost/all.hpp>
#include <iostream>
using namespace std;

int main() {
    cout << "initially, roomba_stop is " << get_config().getBool("roomba_stop")
         << endl;
    // this json file has roomba_stop and some other variables
    set_extra_config("./ihsboost_config.json"); // load ihsboost variables from
                                                // the given json file
    cout << "we've successfully configured some ihsboost variables!" << endl;
    cout << "now, roomba_stop is " << get_config().getBool("roomba_stop")
         << endl;
    return 0;
}