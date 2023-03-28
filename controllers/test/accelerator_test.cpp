#include "test.hpp"
#include <ihsboost/controllers.hpp>
#include <iostream>
#include <math.h>
using namespace std;

void test_linear(int start, int end, double dps, int ups)
{
    double delta_per_update = dps / ups;
    LinearController l(start, end, dps, ups);

    for (double i = start; i <= end; i += delta_per_update)
    {
        assert_equals(i, l.speed(), "linear checking");
        l.step();
    }
}

int main()
{
    test_linear(0, 100, 1000, 1000);
    test_linear(0, 100, 100, 1000);
    test_linear(50, -50, 200, 1000);
    test_linear(-1000, 1000, 1000, 10);

    return 0;
}