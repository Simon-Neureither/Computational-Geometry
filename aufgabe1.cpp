

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

class Point
{
    float x, y;
public:
    Point(float x, float y) : x(x), y(y) {}

    const float getX() const { return x; }
    const float getY() const { return y; }

    friend std::ostream& operator<<(std::ostream&, const Point&);

    

};

bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY();
}
bool operator!=(const Point& lhs, const Point& rhs)
{
    return lhs.getX() != rhs.getX() || lhs.getY() != rhs.getY();
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

    float ccw(Point r)
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
    IntersectionInfo(Line l1, Line l2, float ccw, float ccw2, float ccw3, float ccw4) : l1(l1), l2(l2), ccw(ccw), ccw2(ccw2), ccw3(ccw3), ccw4(ccw4) {}
    Line l1;
    Line l2;
    float ccw;
    float ccw2;
    float ccw3;
    float ccw4;
};



long get_ms(const std::chrono::steady_clock::time_point p, const std::chrono::steady_clock::time_point p2)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(p2 - p).count();
}

int main(void)
{
    auto start_time = std::chrono::steady_clock::now();
 
    std::vector<Line> lines;

    std::ifstream input("s_10000_1.dat", std::ios_base::in);

    float x, y, x2, y2;

    if (!input.is_open())
        return 1;

    std::string textline;

    
   
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

                float ccw1 = line1.ccw(line2.getStart());
                float ccw2 = line1.ccw(line2.getEnd());
                float ccw3 = line2.ccw(line1.getStart());
                float ccw4 = line2.ccw(line1.getEnd());

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

    std::ofstream out("log.txt", std::ios_base::out);
    for (int i = 0; i < intersecting_lines.size(); i++)
    {
        auto info = intersecting_lines[i];
        out << "Intersecting: " << info.l1 << " " << info.l2 << " " << info.ccw << " " << info.ccw2 << " " << info.ccw3 << " " << info.ccw4 << std::endl;
    }

    out.close();




    auto end_time = std::chrono::steady_clock::now();

    std::cout << "Time taken total: " << get_ms(start_time, end_time) << " milliseconds." << std::endl;
    std::cout << "Time taken for parsing input: " << get_ms(start_time, finished_input_time) << " milliseconds." << std::endl;
    std::cout << "Time taken for generating output: " << get_ms(finished_input_time, finished_generating_info_time) << " milliseconds." << std::endl;
    std::cout << "Time taken for writing output: " << get_ms(finished_generating_info_time, end_time) << " milliseconds." << std::endl;

    std::cout << "Number of intersections " << intersecting_lines.size() << "." << std::endl;

    getchar();

    return 0;
}