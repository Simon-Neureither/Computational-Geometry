#pragma once

#include <ostream>


class Point
{
    double x, y;


public:
    Point()
    {
        x = 0;
        y = 0;
    }
    Point(double x, double y) : x(x), y(y) {}

    inline auto getX() const { return x; }
    inline auto getY() const { return y; }

    friend std::ostream& operator<<(std::ostream& out, const Point& point)
    {
        out << "(" << point.getX() << ", " << point.getY() << ")";
        return out;
    }
};

