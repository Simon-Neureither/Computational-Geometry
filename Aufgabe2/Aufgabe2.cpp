

#include <iostream>
#include <istream>

#include <fstream>

#include <vector>

#include <string>

#include <map>

#include <math.h>

#include <algorithm>

const double GERMANY_WIDTH = 640; // km
const double GERMANY_HEIGHT = 876; // km
const double BAVARIA = 70550; // km^2
const double SAXONY = 18416; // km^2
const double GERMANY = 357386; // km^2

class Point
{
    double x, y;
public:
    Point(double x, double y) : x(x), y(y) {}

    const double getX() const { return x; }
    const double getY() const { return y; }

    void setX(double x_new) { x = x_new;}
    void setY(double y_new) { y = y_new;}

    friend std::ostream& operator<<(std::ostream&, const Point&);
};


double ccw(const Point& p, const Point& q, const Point& r)
{
    return (p.getX() * q.getY() - p.getY() * q.getX())
        + (q.getX() * r.getY() - q.getY() * r.getX())
        + (p.getY() * r.getX() - p.getX() * r.getY());
}

int sgn(double x)
{
    if (x == 0)
        return 0;
    return x / abs(x);
}

class Polygon
{
    std::vector<Point> m_points;

    double sum_x;
    double sum_y;

public:
    
    void AddPoint(double x, double y)
    {
        m_points.push_back(Point(x, y));

        sum_x += x;
        sum_y += y;
    }

    void AddRelativePoint(double x, double y)
    {
        if (m_points.size() == 0)
        {
            std::cout << "Error." << std::endl;
            return;
        }
        
        Point p = m_points[m_points.size() - 1];
        m_points.push_back(Point(p.getX() + x, p.getY() + y));

        sum_x += p.getX() + x;
        sum_y += p.getY() + y;
    }

    double area()
    {
        double area = 0;
        for (int i = 1; i < m_points.size() - 1; i++)
        {
            
            area += m_points[i].getY() * (m_points[i - 1].getX() - m_points[i + 1].getX()) / 2;
            
        }
        return abs(area);
    }

    Point getCenter()
    {
        return Point(sum_x / m_points.size(), sum_y / m_points.size());
    }

    Point getFirstPoint()
    {
        return m_points[0];
    }

    bool IsInside(Point q)
    {
        Point p_dach(0, 0);

        int i = 0;

        while (ccw(p_dach, q, m_points[i]) == 0)
        {
            i++;
        }

        int s = 0;
        
        int lr = sgn(ccw(p_dach, q, m_points[i]));

        for (int j = i + 1; j < m_points.size(); j++)
        {
            int lrnew = sgn(ccw(p_dach, q, m_points[j]));
            if (abs(lrnew - lr) == 2)
            {
                lr = lrnew;
                if (ccw(m_points[j - 1], m_points[j], p_dach) * ccw(m_points[j - 1], m_points[j], q) <= 0)
                {
                    s++;
                }
            }
        }

        return s % 2;
    }
};

// TODO: check if it works as intended
// Remove duplicate areas from polygons e.g. berlin from Brandenburg
void removeDuplicateAreas(std::vector<Polygon> *polygonList){
    std::vector<int> markedPolys;
    for(int i = 0; i < polygonList->size(); i++){
        for(int j = 0; j < polygonList->size(); j++){
            if( i != j && (*polygonList)[j].IsInside((*polygonList)[i].getFirstPoint())){
                markedPolys.push_back(i);
            }    
        }
    }

    for(auto poly_index : markedPolys){
        polygonList->erase(polygonList->begin() + poly_index);
    }
}

// Assign each capital city to its corresponding state
void getCapitalCities(
    std::map<std::string, std::vector<Polygon>> state_to_surface,
    std::map<std::string, Point> city_to_center, 
    std::map<std::string, std::string> *state_to_capital)
{
    //Vars for duplicate check
    std::vector<std::string> duplicateCapitals;   
    std::map<std::string, std::vector<std::string>> temp; 
    for(auto c : city_to_center){
        uint32_t count = 0;
        for(auto s: state_to_surface){
            for(int i = 0; i < s.second.size(); i++){
                if(s.second[i].IsInside(c.second)){
                    if(temp.find(s.first) != temp.end()){
                        temp[s.first].push_back(c.first);
                    }else {
                        temp.emplace(s.first, std::vector<std::string>());
                        temp[s.first].push_back(c.first); 
                    }  
                    count++;  
                }
            } 
        }
        if(count > 1){
            duplicateCapitals.push_back(c.first);
        }
    }

    for(auto t : temp){
        // Remove duplicate capitals 
        if(t.second.size() > 1){
            for(auto d : duplicateCapitals){
                auto it = std::find(t.second.begin(), t.second.end(), d);
                if(it != t.second.end()){
                   t.second.erase(it);
                   state_to_capital->emplace(t.first, t.second[0]);   
                }
            }
        }else {
            state_to_capital->emplace(t.first, t.second[0]);
        }
    }
}


int main(void)
{

    std::ifstream input("DeutschlandMitStaedten.svg", std::ios_base::in);

    double x, y, x2, y2;

    if (!input.is_open())
        return 1;

    std::string textline;

    std::map<std::string, std::vector<Polygon>> state_to_surface;
    std::map<std::string, Point> city_to_center;
    std::map<std::string, std::string> state_to_capital;

    std::vector<Polygon>* polygonList;
    Polygon polygon;

    std::string id;

    std::ofstream out("log_aufgabe2.txt", std::ios_base::out);

    int pos;
    bool parse_cities = false;

    while (std::getline(input, textline))
    {
        if (textline.empty())
            break;

        // Parse states 
        pos = textline.find("path id=\"");
        
        if (pos != std::string::npos)
        {
            int end = textline.find("\"", pos + 9);

            id = textline.substr(pos + 9, end - pos - 9);

            state_to_surface.emplace(id, std::vector<Polygon>());

            polygonList = &state_to_surface[id];
        }
        else
        {

            char first = textline[0];
            if (first == 'M' || first == 'l' || first == 'L')
            {
                textline = textline.substr(1);

                int pos = textline.find(",");

                double x, y;

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

        // End of states
        pos = textline.find("</g>");

        if(pos != std::string::npos && !parse_cities)
            parse_cities = true;

        // Parse cities
        if(parse_cities){
            pos = textline.find("id=\"");

            if (pos != std::string::npos){
                 int end = textline.find("\"", pos + 4);

                 std::string id_city;   
                 double x_city, y_city;

                 id_city = textline.substr(pos + 4, end - pos - 4);

                 std::string nextLine;
                 std::getline(input, nextLine);
                 pos = nextLine.find("cx=\"");

                 if (pos != std::string::npos){
                     int end = nextLine.find("\"", pos + 4);
                     x_city = std::stod(nextLine.substr(pos + 4, end - pos - 4));   
                 }
                 std::getline(input, nextLine);
                 pos = nextLine.find("cy=\"");

                if (pos != std::string::npos){
                    int end = nextLine.find("\"", pos + 4);
                    y_city = std::stod(nextLine.substr(pos + 4, end - pos - 4));
                }  

                city_to_center.emplace(id_city, Point(x_city, y_city));               
            }
        }    
    }

    for (auto pair : city_to_center) {
       //std::cout << pair.first << ": " << pair.second.getX() << " , " << pair.second.getY() <<  std::endl;
    }

    out.close();

   // Remove areas inside areas of a polygon
   for (auto& p: state_to_surface){
       removeDuplicateAreas(&p.second);
   }

    double germany_area = 0;

    std::map<std::string, double> state_to_area;

    for (auto pair : state_to_surface)
    {
        double area = 0;

        for (int i = 0; i < pair.second.size(); i++)
        {
            double poly_area = pair.second[i].area();
            area += poly_area;
        }

        state_to_area[pair.first] = area;
        germany_area += area;
    }

    // Scale
    double scale = GERMANY / germany_area;

    for (auto pair : state_to_area)
    {
        state_to_area[pair.first] = pair.second * scale;
        std::cout << pair.first << "'s Fläche beträgt: " << state_to_area[pair.first] << " km^2" << std::endl;
    }
    germany_area *= scale;

    std::cout << "Deutschlands Fläche beträgt: " << germany_area << " km^2" << std::endl;

    std::cout << std::endl;

    getCapitalCities(state_to_surface,city_to_center, &state_to_capital);

    for(auto s :  state_to_capital){
        std::cout << s.second << " ist die Hauptstadt von " << s.first << std::endl;
    }

    getchar();
}