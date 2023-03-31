/**
 * @file encoder_example.cpp
 * @author Eliot Hall
 * @brief File that demonstrates use of encoder functions
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <ihsboost/all.hpp>
#include <iostream>
#include <kipr/wombat.h>
#include <kipr/wombat.hpp>

using namespace std;

int main()
{
    create_connect_once();
    kipr::create::Create::instance()->setRefreshRate(8); // set the refresh rate to slightly below 10

    // durability test
    for (int i = 0; i < 10; ++i)
    {
        encoder_drive_straight(500, 150);
        encoder_turn_degrees(300, 180 * (i % 2 == 0 ? 1 : -1));
    }

    create_disconnect();
    return 0;
}