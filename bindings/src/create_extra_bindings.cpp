#include "create_extra.hpp"
#include <boost/python.hpp>

void export_create_extra()
{
    using namespace boost::python;
    // create_extra
    class_<CreateExtraController>("CreateExtraController", init<>())
        .def("run_main_brush", &CreateExtraController::run_main_brush)
        .def("run_side_brush", &CreateExtraController::run_side_brush)
        .def("run_vacuum", &CreateExtraController::run_vacuum);
}