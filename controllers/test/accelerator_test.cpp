#include "test.hpp"
#include "controllers.hpp"
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

void test_sinusoidal(int start, int end, double dps, int ups)
{
    double necessary_updates = abs((end - start) * ups / dps);
    SinusoidalController s(start, end, dps, ups);

    for (int i = 0; i < necessary_updates; ++i)
    {
        assert_equals(start + (end - start) * sin(i / necessary_updates * M_PI / 2), s.speed(), "sinusoidal checking");
        s.step();
    }
}

int main()
{
    test_linear(0, 100, 1000, 1000);
    test_linear(0, 100, 100, 1000);
    test_linear(50, -50, 200, 1000);
    test_linear(-1000, 1000, 1000, 10);

    test_sinusoidal(0, 100, 1000, 1000);
    test_sinusoidal(0, 100, 100, 1000);
    test_sinusoidal(50, -50, 200, 1000);
    test_sinusoidal(-1000, 1000, 1000, 10);
    return 0;
}