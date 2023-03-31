#include <boost/python.hpp>

void export_communicate();
void export_create_extra();
void export_movement();
void export_servos();
void export_util();

BOOST_PYTHON_MODULE(ihs_bindings)
{
#ifdef COMMUNICATE
    export_communicate();
#endif
#ifdef CREATE_EXTRA
    export_create_extra();
#endif
#ifdef MOVEMENT
    export_movement();
#endif
#ifdef SERVOS
    export_servos();
#endif
#ifdef UTIL
    export_util();
#endif
};