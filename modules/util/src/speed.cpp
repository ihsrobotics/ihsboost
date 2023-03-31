#include "speed.hpp"

Speed::Speed(int left_speed, int right_speed) : left(left_speed), right(right_speed){};

Speed Speed::operator-(const Speed o) const
{
    return Speed(left - o.left, right - o.right);
}
Speed Speed::operator+(const Speed o) const
{
    return Speed(left + o.left, right + o.right);
}
Speed &Speed::operator+=(const Speed o)
{
    left += o.left;
    right += o.right;
    return *this;
}
Speed &Speed::operator-=(const Speed o)
{
    left -= o.left;
    right -= o.right;
    return *this;
}

bool Speed::operator==(const Speed o) const
{
    return left == o.left && right == o.right;
}
bool Speed::operator!=(const Speed o) const
{
    return !(*this == o);
}