#include "InputReader.hpp"
#include <iostream>

std::vector<Line> readProgram(std::istream& in, bool isStdin)
{
    std::vector<Line> program;
    std::string line;
    int lineNumber = 0;

    while (std::getline(in, line))
    {
        lineNumber++;
        if (isStdin)
        {
            if (line == ";;")
                break;
        }
        program.push_back(Line{lineNumber, line});
    }

    return program;
}
