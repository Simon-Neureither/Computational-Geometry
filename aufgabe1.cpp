

#include <iostream>
#include <istream>

#include <fstream>

#include <vector>
#include <tuple>

#include <string>
#include <chrono>

#include <math.h>

#define min(x,y) x > y ? y : x
#define max(x,y) x > y ? x : y

class Double
{
private:
    double d;
public:
    Double(double d) : d(d) {}
    Double() : d(0) {}
<<<<<<< HEAD

=======
    
>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab
    friend bool operator==(const Double& d, const Double& d2);

    friend Double operator*(const Double&d, const Double& d2);
    friend bool operator>=(const Double& d, const Double& d2);
    friend bool operator<=(const Double& d, const Double& d2);

    friend bool operator<(const Double& d, const Double& d2);
    friend bool operator>(const Double& d, const Double& d2);

    friend bool operator!=(const Double& d, const Double& d2);

    friend Double operator+(const Double&d, const Double& d2);
    friend Double operator-(const Double&d, const Double& d2);
    friend Double operator/(const Double&d, const Double& d2);

    friend std::ostream& operator<<(std::ostream&, const Double& d);

    Double& operator*=(const Double&d);

    friend auto sin(const Double&d);
    friend auto cos(const Double&d);
    friend auto abs(const Double&d);

<<<<<<< HEAD
    auto get()
=======
    double get()
>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab
    {
        return d;
    }
};

#define EPSILON DBL_EPSILON * 100

bool operator==(const Double&d, const Double&d2)
{
    return fabs(d.d - d2.d) < EPSILON;
}

Double operator*(const Double&d, const Double&d2)
{
    return d.d*d2.d;
}

bool operator>=(const Double&d, const Double&d2)
{
    return d.d - d2.d >= -EPSILON;
}
bool operator<=(const Double&d, const Double&d2)
{
    return d.d - d2.d <= EPSILON;
}
bool operator>(const Double&d, const Double&d2)
{
    return d.d - d2.d > -EPSILON;
}
bool operator<(const Double&d, const Double&d2)
{
    return d2 > d;
}

bool operator!=(const Double&d, const Double&d2)
{
    return !(d == d2);
}
Double operator+(const Double&d, const Double&d2)
{
<<<<<<< HEAD
    return d.d + d2.d;
=======
    return d.d+d2.d;
>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab
}

Double operator-(const Double&d, const Double&d2)
{
<<<<<<< HEAD
    return d.d - d2.d;
=======
    return d.d-d2.d;
>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab
}
Double operator/(const Double&d, const Double&d2)
{
    return d.d / d2.d;
}

std::ostream& operator<<(std::ostream& out, const Double& d)
{
    out << d.d;
    return out;
}

Double& Double::operator*=(const Double&d)
{
    this->d *= d.d;
    return *this;
}

class Point
{
    Double x, y;
public:
    Point(Double x, Double y) : x(x), y(y) {}
    Point(const Point& p) : x(p.x), y(p.y) {}

    const Double getX() const { return x; }
    const Double getY() const { return y; }

    friend std::ostream& operator<<(std::ostream&, const Point&);



};

bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY();
}
bool operator!=(const Point& lhs, const Point& rhs)
{
    return lhs.getX() != rhs.getX() || lhs.getY() != rhs.getY();
}

auto sin(const Double&d)
{
    return sin(d.d);
}

auto cos(const Double&d)
{
    return cos(d.d);
}
auto abs(const Double&d)
{
    return abs(d.d);
}



class Line
{
    Point start;
    Point end;

    Point p_min;
    Point p_max;

public:
    Line(Point q, Point p) : start(q), end(p),
        p_min(Point(min(start.getX(), end.getX()), min(start.getY(), end.getY()))),
        p_max(Point(max(start.getX(), end.getX()), max(start.getY(), end.getY())))
    {}

    const Point getStart() const { return start; }
    const Point getEnd() const { return end; }

    Double ccw(const Point& r)
    {

        return (start.getX() * end.getY() - start.getY() * end.getX()) + (end.getX() * r.getY() - end.getY() * r.getX()) + (start.getY() * r.getX() - start.getX() * r.getY());
    }

    friend std::ostream& operator<<(std::ostream& out, const Line& line);


    // Works only if colinear
    bool IsBetween(const Point& point)
    {
        //   auto p_min = Point(min(start.getX(), end.getX()), min(start.getY(), end.getY()));
        //   auto p_max = Point(max(start.getX(), end.getX()), max(start.getY(), end.getY()));

        return point.getX() >= p_min.getX()
            && point.getY() >= p_min.getY()
            && point.getX() <= p_max.getX()
            && point.getY() <= p_max.getY();
    }
};

std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out << "(" << point.getX() << ", " << point.getY() << ")";
    return out;
}


std::ostream& operator<<(std::ostream& out, const Line& line)
{
    out << line.getStart() << " " << line.getEnd();
    return out;
}

class IntersectionInfo
{
public:
    IntersectionInfo(Line l1, Line l2, Double ccw, Double ccw2, Double ccw3, Double ccw4) : l1(l1), l2(l2), ccw(ccw), ccw2(ccw2), ccw3(ccw3), ccw4(ccw4) {}
    Line l1;
    Line l2;
    Double ccw;
    Double ccw2;
    Double ccw3;
    Double ccw4;
};



long get_ms(const std::chrono::steady_clock::time_point p, const std::chrono::steady_clock::time_point p2)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(p2 - p).count();
}


// For checks only.
typedef Point Slope;

Point rotate(Point p, Double angle)
{
    angle = 3.14159265359 / 180 * angle;
    Point q(p.getX() * cos(angle) - p.getY() * sin(angle), p.getY() * cos(angle) + p.getX() * sin(angle));
    return q;
}
Point normalize(Point p)
{
    Double sum = abs(p.getX()) + abs(p.getY());


    Double x = p.getX() / sum;
    Double y = p.getY() / sum;
<<<<<<< HEAD

=======
    
>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab
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

    return getPointOfIntersection(p, q, s, t);
}



Point getPointOfIntersection(Point p, Point q, Point s, Point t)
{

    Slope pq = normalize(getSlope(p, q));
    Slope st = normalize(getSlope(s, t));

    Double B = ((s.getY() - p.getY()) * pq.getX() - (s.getX() - p.getX()) * pq.getY()) / ((st.getX() * pq.getY() - st.getY() * pq.getX()));
    Double A = (s.getY() + B * st.getY() - p.getY()) / pq.getY();

    // nan because of: st.getX() * pq.getY() == st.getY() * pq.getX()
    // because normalized slope is used that means:
    // ??? TODO
    if (std::isnan(B.get()) || std::isnan(A.get()))
    {
        p = rotate(p, 90);
        q = rotate(q, 90);
        s = rotate(s, 90);
        t = rotate(t, 90);
        Point rot = getPointOfIntersection(p, q, s, t);
        rot = rotate(rot, -90);
        return rot;
    }

    if (Point(p.getX() + A * pq.getX(), p.getY() + A * pq.getY()) != Point(s.getX() + B * st.getX(), s.getY() + B * st.getY()))
    {
<<<<<<< HEAD
        // __debugbreak();
       // std::cout << "Error: " << p << " " << q << " " << s << " " << t << " produces intersections: " << Point(p.getX() + A * pq.getX(), p.getY() + A * pq.getY())
       //     << " and " << Point(s.getX() + B * st.getX(), s.getY() + B * st.getY()) << std::endl;
=======
       // __debugbreak();
        std::cout << "Error: " << p << " " << q << " " << s << " " << t << " produces intersections: " << Point(p.getX() + A * pq.getX(), p.getY() + A * pq.getY())
            << " and " << Point(s.getX() + B * st.getX(), s.getY() + B * st.getY()) << std::endl;
>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab
    }

    return Point(p.getX() + A * pq.getX(), p.getY() + A * pq.getY());
}


bool test_line(Line l, Line l2)
{
    Slope s = getSlope(l.getStart(), l.getEnd());
    Slope s2 = getSlope(l2.getStart(), l2.getEnd());

    Slope sn = normalize(s);
    Slope s2n = normalize(s2);

    // Check NORMALIZED slope for equality (parallelism).
    // (in this case x >= 0 and if x = 0, y >= 0; x + y = 1;
    // does work here with NAN since two NANs are not equal.
    // (NAN -> lines are really points l.start = l.end, ...)
    if (sn.getX() == s2n.getX() && sn.getY() == s2n.getY())
    {
        // parallel.


        // Two cases
        // case 1: one of the points of line2 is between line1's points -> intersection found
        // case 2: no points of line2 is between line1's points -> no intersection

        // Check can be done with a bounding box, since all points are on the same line.

        if (l.IsBetween(l2.getStart()) || l.IsBetween(l2.getEnd()))
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    else if (abs(s.getX()) + abs(s.getY()) == 0 || abs(s2.getX()) + abs(s2.getY()) == 0)
    {
        // Both lines contain only one point each.
        // If the points are equals, the lines intersect.
        return l.getStart() == l2.getStart();
    }
    else if (abs(s.getX()) + abs(s.getY()) == 0)
    {
        // Line l1 is only a point. Check if point is on l2.

        // Equation to solve:
        // l.getStart() + A * s = l2.getStart()
        // <=> A = (l2.getStart() - l.getStart()) / s    <-- at this point |s| != 0. 
        // <=> A = (l2.getStart().getX() - l.getStart().getX()) / s.getX();
        // <=> A = (l2.getStart().getY() - l.getStart().getY()) / s.getY();

        // if A is the same, the point is on the line, so a check for the bounding box of the line
        // segment is sufficient

        Double Ax = (l2.getStart().getX() - l.getStart().getX()) / s.getX();
        Double Ay = (l2.getStart().getY() - l.getStart().getY()) / s.getY();

        // Either Ax or Ay (or both) are not +/- infinite (since at least one of s' components are != 0)

        if (Ax == Ay)
        {
            return l.IsBetween(l2.getStart());
        }
        else
        {
            return false;
        }

    }
    else if (abs(s2.getX()) + abs(s2.getY()) == 0)
    {
        // Line l2 is only a point. Check if point is on l1.

        // Equation to solve:
        // l2.getStart() + A * s2 = l.getStart()

        Double Ax = (l.getStart().getX() - l2.getStart().getX()) / s2.getX();
        Double Ay = (l.getStart().getY() - l2.getStart().getY()) / s2.getY();

        // Either Ax or Ay (or both) are not +/- infinite (since at least one of s2' components are != 0)

        if (Ax == Ay)
        {
            return l2.IsBetween(l.getStart());
        }
        else
        {
            return false;
        }
    }
    else
    {
        Point intersection = getPointOfIntersection(l, l2);

        // Check if point is in bounding box.
        // Bounding box can be used since it is known that the point is located on both lines.

<<<<<<< HEAD
=======
        Double Ax = (l.getStart().getX() - l2.getStart().getX()) / s2n.getX();
        Double Ay = (l.getStart().getY() - l2.getStart().getY()) / s2n.getY();

        Double Bx = (l2.getStart().getX() - l.getStart().getX()) / sn.getX();
        Double By = (l2.getStart().getY() - l.getStart().getY()) / sn.getY();

>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab
        if (l.IsBetween(intersection) && l2.IsBetween(intersection))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}


int main(void)
{
    auto start_time = std::chrono::steady_clock::now();

    std::vector<Line> lines;

    std::ifstream input("s_100000_1.dat", std::ios_base::in);

    Double x, y, x2, y2;

    if (!input.is_open())
        return 1;


<<<<<<< HEAD
=======
    Line a(Point(56.857, 93.13), Point(56.7833, 92.9337));
    Line b(Point(56.925, 93.478), Point(56.7926, 92.7889));

    Line c(Point(61.637, 67.322), Point(62.0178, 67.5628));
    Line d(Point(61.77, 67.501), Point(61.77, 67.0229));

    // WRONG/DIFFERENT:
    
    /*
    ccw has it, test not
    Intersecting: (61.637, 67.322) (62.0178, 67.5628) (61.77, 67.501) (61.77, 67.0229)
    Intersecting: (92.852, 3.902) (92.3061, 3.902) (93.041, 3.625) (92.1667, 4.39469)
    */

    test_line(c, d);

    {
        Line line1 = c;
        Line line2 = d;

        Double ccw1 = line1.ccw(line2.getStart());
        Double ccw2 = line1.ccw(line2.getEnd());
        Double ccw3 = line2.ccw(line1.getStart());
        Double ccw4 = line2.ccw(line1.getEnd());

        Double ccw1_2 = line1.ccw(line2.getStart()) * line1.ccw(line2.getEnd());
        Double ccw3_4 = line2.ccw(line1.getStart()) * line2.ccw(line1.getEnd());

        Point intersection = getPointOfIntersection(line1, line2);

        Point p = line1.getStart();
        Point q = line1.getEnd();

        Point s = line2.getStart();
        Point t = line2.getEnd();


        if (ccw1 == 0 && ccw2 == 0 && ccw3 == 0 && ccw4 == 0)
        {

            if (line1.IsBetween(line2.getStart()) || line1.IsBetween(line2.getEnd()))
                ;// push_back(IntersectionInfo(line1, line2, ccw1, ccw2, ccw3, ccw4));

        }
        else if (ccw1 * ccw2 <= 0 && ccw3 * ccw4 <= 0)
        {
            ;// intersecting_lines.push_back(IntersectionInfo(line1, line2, ccw1, ccw2, ccw3, ccw4));
        }

    }

    test_line(a, b);

    std::string textline;

>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab


    std::string::size_type sz;

    while (std::getline(input, textline))
    {
        if (textline.empty())
            break;


        x = std::stof(textline, &sz);
        textline = textline.substr(sz);
        y = std::stof(textline, &sz);
        textline = textline.substr(sz);
        x2 = std::stof(textline, &sz);
        textline = textline.substr(sz);
        y2 = std::stof(textline, &sz);

        lines.push_back(Line(Point(x, y), Point(x2, y2)));
    }

    auto finished_input_time = std::chrono::steady_clock::now();

    std::vector<IntersectionInfo> intersecting_lines;

    for (int i = 0; i < lines.size(); i++)
    {
        //std::cout << "Testing line " << i << std::endl;
        for (int j = i + 1; j < lines.size(); j++)
        {
            Line line1 = lines[i];
            Line line2 = lines[j];

            Double ccw1 = line1.ccw(line2.getStart());
            Double ccw2 = line1.ccw(line2.getEnd());
            Double ccw3 = line2.ccw(line1.getStart());
            Double ccw4 = line2.ccw(line1.getEnd());

            if (ccw1 == 0 && ccw2 == 0 && ccw3 == 0 && ccw4 == 0)
            {

                if (line1.IsBetween(line2.getStart()) || line1.IsBetween(line2.getEnd()))
                    intersecting_lines.push_back(IntersectionInfo(line1, line2, ccw1, ccw2, ccw3, ccw4));

            }
            else if (ccw1 * ccw2 <= 0 && ccw3 * ccw4 <= 0)
            {
                intersecting_lines.push_back(IntersectionInfo(line1, line2, ccw1, ccw2, ccw3, ccw4));
            }
        }
    }

    auto finished_generating_info_time = std::chrono::steady_clock::now();

    // Test
    std::vector<IntersectionInfo> intersecting_lines_test;

    int tested = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = i + 1; j < lines.size(); j++)
        {
            Line line1 = lines[i];
            Line line2 = lines[j];

            bool b = test_line(line1, line2);
            if (b)
            {
                tested++;
<<<<<<< HEAD
                // std::cout << "Intersecting: " << line1 << " " << line2 << std::endl;
=======
               // std::cout << "Intersecting: " << line1 << " " << line2 << std::endl;
>>>>>>> 4ae8c11445cdb55f0f955303f9092a0f6a2ee7ab
                intersecting_lines_test.push_back(IntersectionInfo(line1, line2, -1, -1, -1, -1));

            }

        }
    }

    auto finished_generating_info_time_test = std::chrono::steady_clock::now();


    std::ofstream out("log.txt", std::ios_base::out);
    for (int i = 0; i < intersecting_lines.size(); i++)
    {
        auto info = intersecting_lines[i];
        out << "Intersecting: " << info.l1 << " " << info.l2 << std::endl;//" " << info.ccw << " " << info.ccw2 << " " << info.ccw3 << " " << info.ccw4 << std::endl;
    }

    out.close();

    std::ofstream out2("log_test.txt", std::ios_base::out);
    for (int i = 0; i < intersecting_lines.size(); i++)
    {
        auto info = intersecting_lines_test[i];
        out2 << "Intersecting: " << info.l1 << " " << info.l2 << std::endl;// " " << info.ccw << " " << info.ccw2 << " " << info.ccw3 << " " << info.ccw4 << std::endl;
    }

    std::cout << "Tested intersection count: " << tested << std::endl;


    out2.close();





    auto end_time = std::chrono::steady_clock::now();

    std::cout << "Time taken total: " << get_ms(start_time, end_time) << " milliseconds." << std::endl;
    std::cout << "Time taken for parsing input: " << get_ms(start_time, finished_input_time) << " milliseconds." << std::endl;
    std::cout << "Time taken for generating output: " << get_ms(finished_input_time, finished_generating_info_time) << " milliseconds." << std::endl;
    std::cout << "Time taken for generating output (test): " << get_ms(finished_generating_info_time, finished_generating_info_time_test) << " milliseconds." << std::endl;
    std::cout << "Time taken for writing output: " << get_ms(finished_generating_info_time_test, end_time) << " milliseconds." << std::endl;

    std::cout << "Number of intersections " << intersecting_lines.size() << "." << std::endl;

    getchar();

    return 0;
}
