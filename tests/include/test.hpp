#include <sstream>
#include <stdexcept>
#include <string>
using std::logic_error;
using std::ostringstream;
using std::string;

class AssertionError : public logic_error {
  public:
    AssertionError(const char *what) : logic_error(what){};
};

template <typename T, typename V>
static void assert_equals(T desired, V given, string where = "main") {
    if (desired != given) {
        ostringstream temp;
        temp << "Error in " << where << ": " << desired << " is not equal to "
             << given;
        throw AssertionError(temp.str().c_str());
    }
}
template <typename T, typename V>
static void assert_notequals(T not_desired, V given, string where = "main") {
    if (not_desired == given) {
        ostringstream temp;
        temp << "Error in " << where << ": " << not_desired << " is equal to "
             << given << " (it shouldn't be)";
        throw AssertionError(temp.str().c_str());
    }
}