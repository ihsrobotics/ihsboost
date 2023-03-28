#include "util.hpp"
#include <boost/python.hpp>
#include <sstream>
#include <string>

namespace util_export
{
    std::string print_speed(Speed &s)
    {
        std::ostringstream o;
        o << "(" << s.left << ", " << s.right << ")";
        return o.str();
    }

    std::string get_version()
    {
        return IHSBOOST_VERSION;
    }
}; // namespace util_export

void export_util()
{
    using namespace util_export;
    using namespace boost::python;

    class_<Speed>("Speed", init<int, int>((arg("left_speed"), arg("right_speed"))))
        .def("__eq__", &Speed::operator==)
        .def("__ne__", &Speed::operator!=)
        .def("__add__", &Speed::operator+)
        .def("__sub__", &Speed::operator-)
        .def_readwrite("left", &Speed::left)
        .def_readwrite("right", &Speed::right)
        .def("__repr__", print_speed);
    class_<Timer>("Timer", init<double>(arg("time")))
        .def("__call__", &Timer::operator())
        .def("done", &Timer::done);
    enum_<Cliff>("Cliff")
        .value("LCliff", Cliff::LCliff)
        .value("LFCliff", Cliff::LFCliff)
        .value("RFCliff", Cliff::RFCliff)
        .value("RCliff", Cliff::RCliff);
    enum_<LineSide>("LineSide")
        .value("LeftSide", LineSide::LeftSide)
        .value("RightSide", LineSide::RightSide);
    def("set_extra_config", set_extra_config);
    def("set_default_config", set_default_config);
    def("get_version", get_version);
}