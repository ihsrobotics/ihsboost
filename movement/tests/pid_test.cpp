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
    instance->setFullMode();
    instance->setRefreshRate(5);
    // encoder_turn_degrees(300, -1080, 11, 1000);
    encoder_drive_straight_pid(200, 75, .09, 0, 0, 11, 1000, 500);
    instance->stop();
    instance->disconnect();
    return 0;
}