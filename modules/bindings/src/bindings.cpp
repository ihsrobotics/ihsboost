#include "modules.hpp"
#include <boost/python.hpp>

void export_communicate();
void export_create_extra();
void export_movement();
void export_servos();
void export_util();

BOOST_PYTHON_MODULE(ihs_bindings)
{
#ifdef build_communicate
    export_communicate();
#endif
#ifdef build_create_extra
    export_create_extra();
#endif
#ifdef build_movement
    export_movement();
#endif
#ifdef build_servos
    export_servos();
#endif
#ifdef build_util
    export_util();
#endif
};