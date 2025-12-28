#pragma once
#include <vector>
#include <string>
#include <istream>

struct Line {
    int no;
    std::string text;
};

std::vector<Line> readProgram(std::istream& in, bool isStdin);
