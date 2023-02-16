#include <ihsboost/all.hpp>
#include <kipr/wombat.h>
#include <iostream>

using namespace std;

int main()
{
    create_connect_once();
    encoder_drive_straight_pid(200, 75, 1, 0, 0, 11);
    create_disconnect();
    return 0;
}