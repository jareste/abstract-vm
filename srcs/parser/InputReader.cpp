#include <iostream>
#include "InputReader.hpp"
#include "../exception/Exception.hpp"

inputReader::inputReader(const std::string& filename, bool isStdin)
{
    this->_lastLineStored = 0;
    if (isStdin)
    {
        this->_file = &std::cin;
    }
    else
    {
        this->_fileStream.open(filename);
        if (!this->_fileStream.is_open())
        {
            throw FailedToOpenFile(filename);
        }
        this->_file = &this->_fileStream;
    }
}

inputReader::~inputReader()
{
    if (this->_fileStream.is_open())
        this->_fileStream.close();
}

size_t inputReader::readProgram(size_t max_lines)
{
    std::string line;
    int initLineNumber = this->_lastLineStored;

    while (std::getline(*this->_file, line))
    {
        this->_lastLineStored++;
        if (this->_file == &std::cin)
        {
            if (line == ";;")
            {
                std::cin.setstate(std::ios::eofbit);
                break;
            }
        }
        this->_lines.push_back(Line{this->_lastLineStored, line});
        if ((this->_lastLineStored - initLineNumber) == max_lines)
            break;
    }

    return this->_lastLineStored - initLineNumber;
}

Line inputReader::getLine()
{
    if (this->_lines.empty())
        return Line{0, ""};

    Line line = std::move(this->_lines.front());
    this->_lines.pop_front();
    return line;
}
