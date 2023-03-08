#include <boost/python.hpp>

void export_communicate();
void export_create_extra();
void export_movement();
void export_servos();
void export_util();

BOOST_PYTHON_MODULE(ihs_bindings)
{
    export_communicate();
    export_create_extra();
    export_movement();
    export_servos();
    export_util();
};