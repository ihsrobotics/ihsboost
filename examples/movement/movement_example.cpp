#include <ihsboost/accelerate.hpp>
#include <kipr/wombat.h>
#include <kipr/wombat.hpp>

int main()
{
    create_connect_once();
    kipr::create::Create::instance()->setRefreshRate(8);

    // linear
    accelerate_forward_linear(0, 500);
    msleep(500);
    accelerate_forward_linear(500, -500);
    msleep(500);
    accelerate_forward_linear(-500, 0);

    // sinusoidal
    accelerate_forward_sin(0, 500);
    msleep(500);
    accelerate_forward_sin(500, -500);
    msleep(500);
    accelerate_forward_sin(-500, 0);

    // turn
    accelerate_linear({0, 0}, {500, -500});
    accelerate_linear({500, -500}, {0, 0});

    create_disconnect();
    return 0;
}