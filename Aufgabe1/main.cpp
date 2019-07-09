
#include <chrono>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "Point.h"
#include "Line.h"

long get_ms(const std::chrono::steady_clock::time_point p, const std::chrono::steady_clock::time_point p2)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(p2 - p).count();
}

bool read_input_file(std::vector<Line>& lines, std::string filename)
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

        lines.push_back(Line(Point(x, y), Point(x2, y2)));
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

#include <map>


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
            if (!read_input_file(lines, filename))
            {
                std::cout << "Failed to read file '" << filename << "'.";
                getchar();
                return;
            }
            auto read_finished_time = std::chrono::steady_clock::now();

            for (auto i = 0; i < lines.size(); i++)
            {
                for (auto j = i + 1; j < lines.size(); j++)
                {
                    Line& line1 = lines[i];
                    Line& line2 = lines[j];
                    if (are_intersecting(lines[i], lines[j]))
                    {
                        intersections.push_back(IntersectionInfo(line1, line2));
                    }
                }
            }

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