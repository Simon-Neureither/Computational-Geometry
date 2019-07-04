#pragma once

#include "Point.h"
#include "Line.h"

// For checks only.
typedef Point Slope;

Point rotate(Point p, double angle)
{
    angle = 3.14159265359 / 180 * angle;
    Point q(p.getX() * cos(angle) - p.getY() * sin(angle), p.getY() * cos(angle) + p.getX() * sin(angle));
    return q;
}
Point normalize(Point p)
{
    double sum = abs(p.getX()) + abs(p.getY());


    double x = p.getX() / sum;
    double y = p.getY() / sum;

    if (x == 0)
        y = abs(y);
    else if (x < 0)
    {
        x *= -1;
        y *= -1;
    }

    return Point(x, y);
}



Slope getSlope(Point p, Point q)
{
    return Slope(q.getX() - p.getX(), q.getY() - p.getY());
}


// fwd decl
Point getPointOfIntersection(Point p, Point q, Point s, Point t);

Point getPointOfIntersection(Line l, Line l2)
{
    Point p = l.getStart();
    Point q = l.getEnd();

    Point s = l2.getStart();
    Point t = l2.getEnd();

    if (p == q)
    {
        return p;
    }

    if (s == t)
    {
        return s;
    }

    if (p == s && q == t)
    {
        return p;
    }
    

    if (p == t && s == q)
    {
        return p;
    }

    return getPointOfIntersection(p, q, s, t);
}



Point getPointOfIntersection(Point p, Point q, Point s, Point t)
{

    Slope pq = normalize(getSlope(p, q));
    Slope st = normalize(getSlope(s, t));

    double B = ((s.getY() - p.getY()) * pq.getX() - (s.getX() - p.getX()) * pq.getY()) / ((st.getX() * pq.getY() - st.getY() * pq.getX()));
    double A = (s.getY() + B * st.getY() - p.getY()) / pq.getY();

    // nan because of: st.getX() * pq.getY() == st.getY() * pq.getX()
    // because normalized slope is used that means:
    // ??? TODO
    if (std::isnan(B) || std::isnan(A))
    {
        /*
        p = rotate(p, 90);
        q = rotate(q, 90);
        s = rotate(s, 90);
        t = rotate(t, 90);
        Point rot = getPointOfIntersection(p, q, s, t);
        rot = rotate(rot, -90);*/
        if (std::isnan(B)) return Point(B, B);
        else
            return Point(A, A);
    }

    return Point(p.getX() + A * pq.getX(), p.getY() + A * pq.getY());
}
