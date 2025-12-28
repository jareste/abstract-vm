#include <iostream>
#include <fstream>
#include "operand/Operand.hpp"
#include "parser/InputReader.hpp"

#if !defined(TEST_OPERAND_MAIN)
int main(int argc, char** argv)
{
    std::vector<Line> program;
    std::cout << "Hello, Abstract VM!" << std::endl;

    if (argc == 1)
        program = readProgram(std::cin, true);
    else
    {
        std::ifstream file(argv[1]);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
        program = readProgram(file, false);
        file.close();
    }

    std::cout << "Read " << program.size() << " lines." << std::endl;
    for (const auto& line : program) {
        std::cout << line.no << ": " << line.text << std::endl;
    }

    Operand<int> op(42, Int32);
    Operand<double> op2(3.14, Double);

    IOperand const * result = op + op2;
    std::cout << "Result: " << result->toString() << std::endl;

    delete result;
    return 0;
}
#endif
