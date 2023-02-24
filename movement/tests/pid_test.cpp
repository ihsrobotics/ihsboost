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
    for (int i = 0; i < 1; ++i)
    {
        encoder_drive_straight_pid(200, 200, .4, 0.25, 0);
        encoder_turn_degrees(300, (i % 2 == 0 ? 1 : -1) * 180);
    }
    instance->stop();
    instance->disconnect();
    return 0;
}