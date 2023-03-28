#include "servos.hpp"
#include <boost/python.hpp>

void export_servos()
{
    using namespace boost::python;
    // servos
    def("move_servo_slowly", move_servo_slowly, (arg("port"), arg("position"), arg("speed") = get_config().getInt("servo_movement_speed"), arg("updates_per_sec") = get_config().getInt("servo_updates_per_sec")));
}