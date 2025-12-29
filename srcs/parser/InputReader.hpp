#pragma once
#include <deque>
#include <string>
#include <istream>
#include <fstream>

struct Line {
    size_t no;
    std::string text;
};

class inputReader {
    private:
        // size_t lastLineRead; // Last one was claimed from the inputReader to process
        size_t _lastLineStored; // Last one was actually readed.
        
        std::deque<Line> _lines;
        
        std::istream* _file;
        std::ifstream _fileStream;
        


        inputReader();
        inputReader(const inputReader & other); // no sense
        const inputReader& operator=(const inputReader& other);


    public:
        inputReader(const std::string& filename, bool isStdin);
        ~inputReader();

        size_t readProgram(size_t max_lines);

        Line getLine();
};


// std::vector<Line> readProgram(std::istream& in, bool isStdin);
