#include <ihsboost/all.hpp>
#include <kipr/wombat.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using kipr::create::Create;
int main()
{
    Create *instance = Create::instance();
    instance->connect();
    instance->setRefreshRate(8);
    // encoder_turn_degrees(300, -1080, 11, 1000);
    for (int i = 0; i < 2; ++i)
    {
        // encoder_drive_straight_pid(200, 75, .05, 0.0007, .0003, 11, 1000, 100);
        encoder_drive_straight_pid(200, 100, 1.1, 0.4, 0, 11, 500, 100);
        encoder_turn_degrees(300, (i % 2 == 0 ? 1 : -1) * 180);
    }
    instance->stop();
    instance->disconnect();
    return 0;
}