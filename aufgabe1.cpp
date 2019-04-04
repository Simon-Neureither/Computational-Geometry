

#include <iostream>
#include <istream>

#include <fstream>

#include <vector>
#include <tuple>

#include <string>

class Point
{
    float x, y;
public:
    Point(float x, float y) : x(x), y(y) {}

    const float getX() const { return x; }
    const float getY() const { return y; }

    friend std::ostream& operator<<(std::ostream&, const Point&);
};

class Line
{
    Point start;
    Point end;

public:
    Line(Point q, Point p) : start(q), end(p) {}

    const Point getStart() const { return start; }
    const Point getEnd() const { return end; }

    float ccw(Point r)
    {

        return (start.getX() * end.getY() - start.getY() * end.getX()) + (end.getX() * r.getY() - end.getY() * r.getX()) + (start.getY() * r.getX() - start.getX() * r.getY());
    }

    friend std::ostream& operator<<(std::ostream& out, const Line& line);
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


int main(void)
{
    std::vector<Line> lines;


    std::ifstream input("E:\\ComputationalGeometry\\s_1000_1.dat", std::ios_base::in);

    float x, y, x2, y2;

    if (!input.is_open())
        return 1;

    //while (!input.eof())

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


        
   //     std::cout << lines.size() << " " << lines[lines.size() - 1] << std::endl;
    }

    std::ofstream out("E:\\ComputationalGeometry\\log.txt", std::ios_base::out);

    for (int i = 0; i < lines.size(); i++)
    {
        std::cout << "Testing line " << i << std::endl;
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
                    out << "Intersecting: " << line1 << " " << line2 << " " << ccw1 << " " << ccw2 << " " << ccw3 << " " << ccw4 << std::endl;
                }
                else if (ccw1 * ccw2 <= 0 && ccw3 * ccw4 <= 0)
                {
                    out << "Intersecting: " << line1 << " " << line2 << " " << ccw1 << " " << ccw2 << " " << ccw3 << " " << ccw4 << std::endl;
                }
        }
    }


    out.close();

    getchar();

    return 0;
}