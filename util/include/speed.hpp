#ifndef SPEED_HPP
#define SPEED_HPP

struct Speed
{
    Speed(int left_speed, int right_speed) : left(left_speed), right(right_speed){};
    int left;
    int right;

    Speed operator-(const Speed o) const;
    Speed operator+(const Speed o) const;
    Speed &operator+=(const Speed o);
    Speed &operator-=(const Speed o);

    bool operator==(const Speed o) const;
    bool operator!=(const Speed o) const;
};

#endif