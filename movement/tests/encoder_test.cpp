#include <ihsboost/all.hpp>
#include <iostream>
#include <kipr/wombat.h>

using namespace std;

int main()
{
    create_connect_once();

    // durability test
    for (int i = 0; i < 10; ++i)
    {
        encoder_drive_straight(500, 150);
        encoder_turn_degrees(300, 180 * (i % 2 == 0 ? 1 : -1));
    }

    create_disconnect();
    return 0;
}