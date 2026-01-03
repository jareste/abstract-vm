#include <iostream>
#include <fstream>
#include <memory>
#include "operand/Operand.hpp"
#include "parser/InputReader.hpp"
#include "parser/Lexer.hpp"
#include "parser/Parser.hpp"
#include "debug_log.hpp"

#if !defined(TEST_OPERAND_MAIN)
// #define PRINT_LINES

int main(int argc, char** argv)
{
    bool isStdin;
    std::string filename;
    size_t linesRead;
    vm virtualMachine;
    Instruction instr;
    std::vector<Token> tokens;
    Line line;

    LOG("Hello, Abstract VM!");

    isStdin = (argc == 1);
    filename = (isStdin) ? "stdin" : argv[1];

    std::unique_ptr<inputReader> iput;
    try
    {
        iput = std::make_unique<inputReader>(filename, isStdin);
    }
    catch (AVMException const& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    linesRead = iput->readProgram(10000);
    while (linesRead > 0)
    {
        LOG("Read " << linesRead << " lines from input.");

        line = iput->getLine();
        LOG("Printing lines read:");
        while ((line.no != 0))
        {
#ifdef PRINT_LINES
            std::cout << "Line " << line.no << ": " << line.text << std::endl;
#endif
            try
            {
                tokens = Lexer::tokenize(line);
            }
            catch (LexicalError const& e)
            {
                std::cerr << e.what() << std::endl;
                std::cerr << "Exiting due to lexical error." << std::endl;
                return 1;
            }

            try
            {
                instr = Parser::parseInstruction(tokens);
            }
            catch (SyntaxError const& e)
            {
                std::cerr << e.what() << std::endl;
                std::cerr << "Exiting due to syntax error." << std::endl;
                return 1;
            }

            if (instr.op == OpCode::None)
            {
                line = iput->getLine();
                continue;
            }

            if (instr.op == OpCode::Exit)
            {
                LOG("Exit instruction encountered. Exiting.");
                return 0;
            }

            try
            {
                virtualMachine.executeInstruction(instr);
            }
            catch (AVMException const& e)
            {
                std::cerr << e.what() << std::endl;
                std::cerr << "Exiting due to VM error." << std::endl;
                return 1;
            }
            catch (std::exception const& e)
            {
                /* stoll */
                std::cerr << "Standard exception: " << e.what() << std::endl;
                std::cerr << "Exiting due to unexpected error." << std::endl;
                return 1;
            }


            line = iput->getLine();
        }

        LOG("End of lines.");
        linesRead = iput->readProgram(10000);
    }

    std::cerr << "No exit instruction found." << std::endl << "Exiting with error." << std::endl;
    return 1;
}
#endif
