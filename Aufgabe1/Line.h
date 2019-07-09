#pragma once

#include "Point.h"

#define min(x,y) (x > y ? y : x)
#define max(x,y) (x > y ? x : y)

class Line
{
    Point m_start;
    Point m_end;

    Point m_min;
    Point m_max;

public:
    Line(Point q, Point p) : m_start(q), m_end(p),
        m_min(Point(min(m_start.getX(), m_end.getX()), min(m_start.getY(), m_end.getY()))),
        m_max(Point(max(m_start.getX(), m_end.getX()), max(m_start.getY(), m_end.getY())))
    {}

    inline const Point getStart() const { return m_start; }
    inline const Point getEnd() const { return m_end; }

    inline auto ccw(const Point& r) const
    {
        return (m_start.getX() * m_end.getY() - m_start.getY() * m_end.getX()) + (m_end.getX() * r.getY() - m_end.getY() * r.getX()) + (m_start.getY() * r.getX() - m_start.getX() * r.getY());
    }

    inline bool is_between(const Point& point) const
    {
        return point.getX() >= m_min.getX()
            && point.getY() >= m_min.getY()
            && point.getX() <= m_max.getX()
            && point.getY() <= m_max.getY();
    }

    friend std::ostream& operator<<(std::ostream& out, const Line& line)
    {
        out << line.getStart() << " " << line.getEnd();
        return out;
    }
};

