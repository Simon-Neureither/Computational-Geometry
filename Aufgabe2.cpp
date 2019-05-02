

#include <iostream>
#include <istream>

#include <fstream>

#include <vector>

#include <string>

#include <map>

class Point
{
    float x, y;
public:
    Point(float x, float y) : x(x), y(y) {}

    const float getX() const { return x; }
    const float getY() const { return y; }

    friend std::ostream& operator<<(std::ostream&, const Point&);
};

class Polygon
{
    std::vector<Point> m_points;
public:
    
    void AddPoint(float x, float y)
    {
        m_points.push_back(Point(x, y));
    }

    void AddRelativePoint(float x, float y)
    {
        if (m_points.size() == 0)
        {
            std::cout << "Error." << std::endl;
            return;
        }
        
        Point p = m_points[m_points.size() - 1];
        m_points.push_back(Point(p.getX() + x, p.getY() + y));
    }

    float area()
    {
        float area = 0;
        for (int i = 1; i < m_points.size() - 1; i++)
        {
            area += m_points[i].getY() * (m_points[i - 1].getX() - m_points[i + 1].getX()) / 2;
        }
        return abs(area);
    }
};


int main(void)
{

    std::ifstream input("DeutschlandMitStaedten.svg", std::ios_base::in);

    float x, y, x2, y2;

    if (!input.is_open())
        return 1;

    std::string textline;


    std::map<std::string, std::vector<Polygon>> state_to_surface;

    std::vector<Polygon>* polygonList;
    Polygon polygon;

    std::string id;

    std::ofstream out("E:\\ComputationalGeometry\\log.txt", std::ios_base::out);


    while (std::getline(input, textline))
    {
        if (textline.empty())
            break;


        int pos = textline.find("path id=\"");
        
        if (pos != std::string::npos)
        {
            int end = textline.find("\"", pos + 9);

            id = textline.substr(pos + 9, end - pos - 9);

            state_to_surface.emplace(id, std::vector<Polygon>());

            polygonList = &state_to_surface[id];

            std::cout << id << std::endl;
        }
        else
        {

            char first = textline[0];
            if (first == 'M' || first == 'l' || first == 'L')
            {
                textline = textline.substr(1);

                int pos = textline.find(",");

                float x, y;

                x = std::stod(textline.substr(0, pos));
                y = std::stod(textline.substr(pos + 1));

                out << first << x << "," << y << std::endl;
                
                switch (first)
                {
                case 'M':
                case 'L':
                    polygon.AddPoint(x, y);
                    break;
                case 'l':
                    polygon.AddRelativePoint(x, y);
                    break;
                }
            }
            else if (first == 'z')
            {
                state_to_surface[id].push_back(polygon);
                polygon = Polygon();
            }
        }


    }

    out.close();

    for (auto it = state_to_surface.begin(); it != state_to_surface.end(); it++)
    {
        float total = 0;
        std::cout << it->first << ": ";
        for (int i = 0; i < it->second.size(); i++)
        {

            float a = it->second[i].area();

            total += (it->second[i].area());
        }

        std::cout << total << std::endl;
    }

    getchar();
}