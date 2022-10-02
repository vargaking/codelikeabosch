// see README.md

#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "world.hpp"

enum class ObjectType
{
    unknown = 0,
    truck = 1,
    car = 2,
    motorbike = 3,
    bicycle = 4,
    pedestrian = 5,
    car_or_truck = 6
};

const double pi = 3.14159265358979;

inline double to_radian (double &degrees)
{
    return (pi * degrees) / 180;
}

class Hitbox
{
    public:
        double a[2], b[2], c[2], d[2];
        double ab_slope, ad_slope, ab_shift, dc_shift, ad_shift, bc_shift;

        Hitbox (double mass_x, double mass_y, double angle, double front, double back, double side) 
        {
            /*front += 100;
            back += 100;
            side += 100;*/
            angle = to_radian(angle);
            double sin = std::sin(angle);
            double cos = std::cos(angle);
            a[0] = mass_x - sin * side - cos * back;
            a[1] = mass_y - cos * side + sin * back;
            b[0] = mass_x + sin * side - cos * back;
            b[1] = mass_y + cos * side + sin * back;
            c[0] = mass_x + sin * side + cos * front;
            c[1] = mass_y + cos * side - sin * front;
            d[0] = mass_x - sin * side + cos * front;
            d[1] = mass_y - cos * side - sin * front;

            ab_slope = (b[0] - a[0]) / (b[1] - a[1]);
            ad_slope = (d[0] - a[0]) / (d[1] - a[1]);

            if (ab_slope == INFINITY) ab_slope = 1000000000;
            if (ad_slope == INFINITY) ad_slope = 1000000000;

            ab_shift = a[0] - (ab_slope * a[1]); 
            dc_shift = d[0] - (ab_slope * d[1]);

            ad_shift = a[0] - (ad_slope * a[1]);
            bc_shift = b[0] - (ad_slope * b[1]);
        }

        bool check_point (double x, double y)
        {
            bool collision = true;
            collision &= (ab_slope * x + ab_shift < y) != (ab_slope * x + dc_shift < y);
            collision &= (ad_slope * x + ad_shift < y) != (ad_slope * x + bc_shift < y);
            return collision;
        }

        bool check_collision (Hitbox that)
        {
            bool collision = false;

            collision |= check_point(that.a[0], that.a[1]);
            collision |= check_point(that.b[0], that.b[1]);
            collision |= check_point(that.c[0], that.c[1]);
            collision |= check_point(that.d[0], that.d[1]);

            collision |= that.check_point(a[0], a[1]);
            collision |= that.check_point(b[0], b[1]);
            collision |= that.check_point(c[0], c[1]);
            collision |= that.check_point(d[0], d[1]);

            return collision;
        }
};

/*int main ()
{
    auto h = Hitbox(2.4, 1, 30, 1, 1, 1);

    std::cout << h.a[0] << " " << h.a[1] << "\n";
    std::cout << h.b[0] << " " << h.b[1] << "\n";
    std::cout << h.c[0] << " " << h.c[1] << "\n";
    std::cout << h.d[0] << " " << h.d[1] << "\n\n\n";

    std::cout << h.ab_slope << " " << h.ad_slope << "\n";

    auto k = Hitbox(0, 0, 0, 1, 1, 1);

    std::cout << h.check_collision(k);
}*/