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

namespace
{

    constexpr size_t kBatchSize = 10000;

    void printLine(const Line& line)
    {
    #ifdef PRINT_LINES
        std::cout << "Line " << line.no << ": " << line.text << "\n";
    #else
        (void)line;
    #endif
    }

    std::unique_ptr<inputReader> makeInput(int argc, char** argv)
    {
        const bool isStdin = (argc == 1);
        const std::string filename = isStdin ? "stdin" : argv[1];
        return std::make_unique<inputReader>(filename, isStdin);
    }

    bool processLine(vm& virtualMachine, const Line& line)
    {
        printLine(line);

        std::vector<Token> tokens = Lexer::tokenize(line);
        Instruction instr = Parser::parseInstruction(tokens);

        if (instr.op == OpCode::None)
            return false;

        if (instr.op == OpCode::Exit)
        {
            LOG("Exit instruction encountered. Exiting.");
            return true;
        }

        virtualMachine.executeInstruction(instr);
        return false;
    }

    bool runProgram(inputReader& input, vm& virtualMachine)
    {
        for (size_t linesRead = input.readProgram(kBatchSize); linesRead > 0; linesRead = input.readProgram(kBatchSize))
        {
            LOG("Read " << linesRead << " lines from input.");

            for (Line line = input.getLine(); line.no != 0; line = input.getLine())
            {
                if (processLine(virtualMachine, line))
                    return true;
            }

            LOG("End of lines.");
        }
        return false;
    }

    int reportAndFail(const char* prefix, const std::exception& e)
    {
        if (prefix && *prefix)
            std::cerr << prefix << ": ";
        std::cerr << e.what() << "\n";
        return 1;
    }

}

int main(int argc, char** argv)
{
    vm virtualMachine;
    
    LOG("Hello, Abstract VM!");

    try
    {
        std::unique_ptr<inputReader> input = makeInput(argc, argv);
        const bool sawExit = runProgram(*input, virtualMachine);

        if (!sawExit)
        {
            std::cerr << "No exit instruction found.\nExiting with error.\n";
            return 1;
        }
        return 0;
    }
    catch (const LexicalError& e)
    {
        std::cerr << e.what() << "\nExiting due to lexical error.\n";
        return 1;
    }
    catch (const SyntaxError& e)
    {
        std::cerr << e.what() << "\nExiting due to syntax error.\n";
        return 1;
    }
    catch (const AVMException& e)
    {
        std::cerr << e.what() << "\nExiting due to VM error.\n";
        return 1;
    }
    catch (const std::exception& e)
    {
        return reportAndFail("Unexpected std::exception", e);
    }
}

#endif
