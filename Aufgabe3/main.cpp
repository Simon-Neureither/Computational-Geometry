
#include <chrono>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "Point.h"
#include "Line.h"

auto get_ms(const std::chrono::steady_clock::time_point p, const std::chrono::steady_clock::time_point p2)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(p2 - p).count();
}



bool read_input_file_sorted(std::vector<Line>& lines, std::string filename)
{
    std::ifstream input(filename, std::ios_base::in);

    if (!input.is_open())
    {
        return false;
    }

    std::string textline;
    std::string::size_type size;

    double x, y, x2, y2;


    while (std::getline(input, textline))
    {
        if (textline.empty())
            break;

        x = std::stof(textline, &size);
        textline = textline.substr(size);
        y = std::stof(textline, &size);
        textline = textline.substr(size);
        x2 = std::stof(textline, &size);
        textline = textline.substr(size);
        y2 = std::stof(textline, &size);

        if (x2 < x || (x2 == x && y2 < y))
        {
            lines.push_back(Line(Point(x2, y2), Point(x, y)));
        }
        else
        {
            lines.push_back(Line(Point(x, y), Point(x2, y2)));
        }
    }

    return true;
}

inline bool are_intersecting(const Line& line1, const Line& line2)
{
    auto ccw1 = line1.ccw(line2.getStart());
    auto ccw2 = line1.ccw(line2.getEnd());
    auto ccw3 = line2.ccw(line1.getStart());
    auto ccw4 = line2.ccw(line1.getEnd());

    if (ccw1 == 0 && ccw2 == 0 && ccw3 == 0 && ccw4 == 0)
    {
        if (line1.is_between(line2.getStart()) || line1.is_between(line2.getEnd()))
        {
            return true;
        }
    }
    else if (ccw1 * ccw2 <= 0 && ccw3 * ccw4 <= 0)
    {
        return true;
    }

    return false;
}

class IntersectionInfo
{
    Line m_line1;
    Line m_line2;
public:
    IntersectionInfo(Line line1, Line line2) : m_line1(line1), m_line2(line2) {}

    Line getLine1() { return m_line1; }
    Line getLine2() { return m_line2; }

};

#include <set>

bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.getX() == rhs.getX() && lhs.getY() == rhs.getY();
}
bool operator==(const Line& line, const Line& line2)
{
    return line.getStart() == line2.getStart()
        && line2.getEnd() == line2.getEnd();
}

int uniqueID = 0;

class Event
{
public:
    Event() : ID(uniqueID++) {}
    ~Event() {}
    virtual double getX() const = 0;
    virtual double getY() const = 0;

    int ID;
};

struct compare
{
    bool operator()(const Event* e, const Event* e2)
    {
        if (e->getX() < e2->getX())
            return true;
        else if (e->getX() == e2->getX())
        {
            if (e->getY() < e2->getY())
                return true;
            else if (e->getY() > e2->getY())
                return false;
            else
            {
                return e->ID < e2->ID;
            }
        }
        else
            return false;
    }
};


class EventLine : public Event
{
protected:
    Line m_line;
public:
    EventLine(Line line) : m_line(line) {}

    Line getLine() const
    {
        return m_line;
    }

};

class EventLineStart : public EventLine
{
public:

    EventLineStart(Line line) : EventLine(line) {}
    double getX() const
    {
        return m_line.getStart().getX();
    }
    double getY() const
    {
        return m_line.getStart().getY();
    }
};

class EventLineEnd : public EventLine
{
public:

    EventLineEnd(Line line) : EventLine(line) {}

    double getX() const
    {
        return m_line.getEnd().getX();
    }
    double getY() const
    {
        return m_line.getEnd().getY();
    }
};

#include "Intersection.h"


class EventIntersection : public Event
{

    Line m_line;
    Line m_line2;

    Point point;
public:

    EventIntersection(Line line, Line line2)
        : m_line(line), m_line2(line2), point(getPointOfIntersection(line, line2)) {}


    double getX() const
    {
        return point.getX();
    }
    double getY() const
    {
        return point.getY();
    }

    Line getLine() const
    {
        return m_line;
    }

    Line getLine2() const
    {
        return m_line2;
    }
};


bool operator<(const Point& lhs, const Point& rhs);


struct intersection_compare
{

    uint64_t get_id(uint64_t id, uint64_t id2)
    {
        uint64_t ret = (max(id, id2) << 32) | (min(id, id2));

        return ret;
    }


    uint64_t get_id(const Line& line, const Line& line2) const
    {
        uint64_t id = line.get_id();
        uint64_t id2 = line2.get_id();

        uint64_t ret = (max(id, id2) << 32) | (min(id, id2));

        return ret;
    }

    bool operator()(const EventIntersection e, const EventIntersection e2) const
    {

        return get_id(e.getLine(), e.getLine2()) < get_id(e2.getLine(), e2.getLine2());
    }
};




bool was_handled(EventIntersection* i, std::set<uint64_t>& handled_intersections)
{
    auto id = intersection_compare().get_id((*i).getLine(), (*i).getLine2());

    if (handled_intersections.find(id) != handled_intersections.end())
    {
        return true;
    }
    else
    {
        handled_intersections.emplace(id);
        return false;
    }
};

void mark_as_handled(EventIntersection * e, std::set<uint64_t>& handled_intersections)
{
    auto id = intersection_compare().get_id((*e).getLine(), (*e).getLine2());

    handled_intersections.emplace(id);
}

void handle_add(std::vector<Line>& sweepLine, EventLineStart* E, std::set<Event*, compare>& eventQueue)
{
    auto line = E->getLine();


    int insert_idx = -1;

    for (int i = 0; i < sweepLine.size(); i++)
    {
        if (line.getStart().getY() < sweepLine[i].getY(E->getX()))
        {
            insert_idx = i;
            break;
        }
    }

    if (insert_idx != -1)
    {
        sweepLine.insert(sweepLine.begin() + insert_idx, line);
    }
    else
    {
        insert_idx = sweepLine.size();
        sweepLine.push_back(line);
    }

    int idx = insert_idx;

    if (idx != -1)
    {
        // above
        if (idx + 1 < sweepLine.size())
        {
            auto line2 = sweepLine[idx + 1];
            if (are_intersecting(line, line2))
            {
                eventQueue.emplace(new EventIntersection(line, line2));
            }
        }
        // below
        if (idx - 1 >= 0)
        {
            auto line2 = sweepLine[idx - 1];
            if (are_intersecting(line, line2))
            {
                eventQueue.emplace(new EventIntersection(line, line2));
            }
        }
    }
}

std::vector<IntersectionInfo> get_intersections(std::vector<Line>& lines, bool print)
{
    std::vector<Line> sweepLine;
    std::set<Event*, compare> eventQueue;

    std::vector<IntersectionInfo> intersections;
    std::set<uint64_t> handled_intersections;


    for (int i = 0; i < lines.size(); i++)
    {
        lines[i].set_id(i);

        EventLineStart* eventStart = new EventLineStart(lines[i]);
        eventQueue.emplace(eventStart);

        EventLineEnd* eventEnd = new EventLineEnd(lines[i]);
        eventQueue.emplace(eventEnd);
    }

    auto start_time = std::chrono::steady_clock::now();


    int last_event_type = -1;

    while (!eventQueue.empty())
    {
        if (get_ms(start_time, std::chrono::steady_clock::now()) > 10000)
        {
            std::cout << "EventQueue: " << eventQueue.size() << std::endl;
            start_time = std::chrono::steady_clock::now();
        }

        Event* e = *eventQueue.begin();
        eventQueue.erase(eventQueue.begin());

        bool err = true;

        if (dynamic_cast<EventLineStart*>(e))
        {
            auto E = ((EventLineStart*)e);  
            handle_add(sweepLine, E, eventQueue);
        }
        else if (dynamic_cast<EventLineEnd*>(e))
        {
            auto E = ((EventLineEnd*)e);

            auto line = E->getLine();

            int idx = -1;

            for (int i = 0; i < sweepLine.size(); i++)
            {
                if (sweepLine[i] == line)
                {
                    idx = i;
                    break;
                }
            }

            if (idx != -1)
            {

                int above = idx + 1;
                int below = idx - 1;

                if (below >= 0 && above < sweepLine.size())
                {
                    auto a = sweepLine[above];
                    auto b = sweepLine[below];

                    if (are_intersecting(a, b))
                    {
                        eventQueue.emplace(new EventIntersection(a, b));
                    }

                }
            }
            

            if (idx == -1 || idx >= sweepLine.size())
            {
                std::cout << "End: Line not found: " << line << std::endl;
            }
            else
            {
                sweepLine.erase(sweepLine.begin() + idx);
            }

        }
        else if (dynamic_cast<EventIntersection*>(e))
        {
            auto E = ((EventIntersection*)e);

            if (std::isnan(E->getX()))
            {
                std::cout << "Skipping Intersection because intersection point is nan!" << std::endl;
            }
            else if (!was_handled(E, handled_intersections))
            {
                mark_as_handled(E, handled_intersections);


                intersections.push_back(IntersectionInfo(E->getLine(), E->getLine2()));

                auto line = E->getLine();
                auto line2 = E->getLine2();

                int idx = -1;
                int idx2 = -1;

                for (int i = 0; i < sweepLine.size(); i++)
                {
                    if (sweepLine[i] == line)
                    {
                        idx = i;
                        break;
                    }
                }

                for (int i = 0; i < sweepLine.size(); i++)
                {
                    if (sweepLine[i] == line2)
                    {
                        idx2 = i;
                        break;
                    }
                }

                int above = max(idx, idx2) + 1;
                int below = min(idx, idx2) - 1;

                if (above < sweepLine.size())
                {
                    auto line = sweepLine[min(idx, idx2)];
                    auto line2 = sweepLine[above];
                    if (are_intersecting(line, line2))
                    {
                        eventQueue.emplace(new EventIntersection(line, line2));
                    }
                }

                if (below >= 0)
                {
                    auto line = sweepLine[max(idx, idx2)];
                    auto line2 = sweepLine[below];
                    if (are_intersecting(line, line2))
                    {
                        eventQueue.emplace(new EventIntersection(line, line2));
                    }
                }



                auto segE1_idx = max(idx, idx2);
                auto segE2_idx = min(idx, idx2);

                auto segE1 = sweepLine[segE1_idx];
                auto segE2 = sweepLine[segE2_idx];

                // swap
                sweepLine[segE1_idx] = segE2;
                sweepLine[segE2_idx] = segE1;
            }
        }
        delete e;
    }
    return intersections;
}

#include <map>

struct Test
{
    double input_ms = 0;
    double running_ms = 0;
    long intersections = 0;
};

Test calcAverage(std::vector<Test> tests)
{
    Test t;

    int size = tests.size();

    if (size > 0)
    {

        for (auto i = 0; i < size; i++)
        {
            t.input_ms += tests[i].input_ms;
            t.running_ms += tests[i].running_ms;
            t.intersections += tests[i].intersections;
        }

        t.input_ms /= size;
        t.running_ms /= size;
        t.intersections /= size;
    }

    return t;
}

void run_test(int runs)
{
    std::vector<std::string> filenames = { "s_1000_10.dat", "s_1000_1.dat", "s_10000_1.dat", "s_100000_1.dat" };
    std::map<std::string, Test> filename_to_average_test_result;
    for (auto i = 0; i < filenames.size(); i++)
    {
        std::string filename = filenames[i];
        std::cout << "Starting test for file '" + filename + "'..." << std::endl;
        std::vector<Test> tests;
        for (auto j = 0; j < runs; j++)
        {
            auto start_time = std::chrono::steady_clock::now();
            std::vector<Line> lines;
            std::vector<IntersectionInfo> intersections;
            if (!read_input_file_sorted(lines, filename))
            {
                std::cout << "Failed to read file '" << filename << "'.";
                getchar();
                return;
            }
            auto read_finished_time = std::chrono::steady_clock::now();

            intersections = get_intersections(lines, false);

            auto calc_intersections_finished_time = std::chrono::steady_clock::now();

            Test test;
            test.input_ms = get_ms(start_time, read_finished_time);
            test.running_ms = get_ms(read_finished_time, calc_intersections_finished_time);
            test.intersections = intersections.size();

            tests.push_back(test);
        }

        Test averageTest = calcAverage(tests);
        filename_to_average_test_result.emplace(filename, averageTest);
    }


    std::cout << "Number of test runs: " << runs << std::endl;
    for (auto i = 0; i < filenames.size(); i++)
    {
        Test t = filename_to_average_test_result[filenames[i]];
        std::cout << "Average test results for file '" << filenames[i] << "':" << std::endl;
        std::cout << "\t input time:                    " << t.input_ms << " ms" << std::endl;
        std::cout << "\t intersection calculation time: " << t.running_ms << " ms" << std::endl;
        std::cout << "\t total time taken:              " << (t.running_ms + t.input_ms) << " ms" << std::endl;
        std::cout << "\t nunber of intersections found: " << t.intersections << std::endl;

    }
}

int main(void)
{
    run_test(20);
    getchar();

    return 0;
}