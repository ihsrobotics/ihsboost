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
    c.set_main_brush_direction_inward();
    c.run_main_brush();
    c.transfer();
    msleep(1000);

    cout << "going outward" << endl;
    c.set_main_brush_direction_outward();
    c.transfer();
    msleep(1000);

    cout << "stopping main brush " << endl;
    c.turn_off_main_brush();
    c.transfer();
    msleep(1000);

    cout << "using side brush CCW" << endl;
    c.set_side_brush_direction_CCW();
    c.run_side_brush();
    c.transfer();
    msleep(1000);

    cout << "using side brush in CW direction" << endl;
    c.set_side_brush_direction_CW();
    c.transfer();
    msleep(1000);

    cout << "stopping side brush" << endl;
    c.turn_off_side_brush();
    c.transfer();
    msleep(1000);

    cout << "vacuuming" << endl;
    c.run_vacuum();
    c.transfer();
    msleep(5000);

    cout << "stopping vacuuming" << endl;
    c.turn_off_vacuum();
    c.transfer();
    msleep(1000);

    create_safe();       // change mode back to safe mode
    create_disconnect(); // cleanup
    return 0;
}