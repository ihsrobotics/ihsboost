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
    instance->setRefreshRate(1);
    encoder_drive_straight_pid(200, 75, .25, 0, 0, 11, 500, 500);
    instance->stop();
    instance->disconnect();
    return 0;
}