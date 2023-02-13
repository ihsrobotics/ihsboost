#include <ihsboost/create_extra.hpp>
#include <iostream>
#include <kipr/wombat.h>
using namespace std;

int main()
{
    create_connect_once();
    create_full(); // change mode to full in order to control vacuum, etc
    CreateExtraController c;

    cout << "waiting to be told to do stuff" << endl;
    string temp;
    cin >> temp;

    cout << "going inward" << endl;
    c.run_main_brush(127);
    msleep(1000);

    cout << "going outward" << endl;
    c.run_main_brush(-127);
    msleep(1000);

    cout << "stopping main brush " << endl;
    c.run_main_brush(0);
    msleep(1000);

    cout << "using side brush CCW" << endl;
    c.run_side_brush(127);
    msleep(1000);

    cout << "using side brush in CW direction" << endl;
    c.run_side_brush(-127);
    msleep(1000);

    cout << "stopping side brush" << endl;
    c.run_side_brush(0);
    msleep(1000);

    cout << "vacuuming" << endl;
    c.run_vacuum(127);
    msleep(5000);

    cout << "stopping vacuuming" << endl;
    c.run_vacuum(0);
    msleep(1000);

    create_safe();       // change mode back to safe mode
    create_disconnect(); // cleanup
    return 0;
}