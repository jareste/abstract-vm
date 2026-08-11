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
    enum class RunMode
    {
        Normal,
        ContinueOnError,
        Validate
    };

    struct RunResult
    {
        bool sawExit;
        bool hadErrors;
    };

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

    RunResult runProgram(inputReader& input, vm& virtualMachine, RunMode mode)
    {
        bool sawExit = false;
        bool hadErrors = false;

        for (size_t linesRead = input.readProgram(kBatchSize);
            linesRead > 0;
            linesRead = input.readProgram(kBatchSize))
        {
            for (Line line = input.getLine();
                line.no != 0;
                line = input.getLine())
            {
                try
                {
                    if (processLine(virtualMachine, line))
                    {
                        sawExit = true;

                        if (mode == RunMode::Normal)
                            return {true, false};
                    }
                }
                catch (const AVMException& e)
                {
                    hadErrors = true;

                    if (mode == RunMode::Normal)
                        throw;

                    std::cerr << e.what() << '\n';
                }
                catch (const std::exception& e)
                {
                    hadErrors = true;

                    if (mode == RunMode::Normal)
                        throw;

                    std::cerr << "Unexpected error on line "
                            << line.no << ": "
                            << e.what() << '\n';
                }
            }
        }

        return {sawExit, hadErrors};
    }

    RunMode parseMode(int argc, char** argv)
    {
        if (argc < 3)
            return RunMode::Normal;

        std::string flag = argv[2];

        if (flag == "--continue-on-error")
            return RunMode::ContinueOnError;

        if (flag == "--validate-file")
            return RunMode::Validate;

        throw InvalidArgument("Unknown option: " + flag);
    }

}

int main(int argc, char** argv)
{
    vm virtualMachine;
    
    LOG("Hello, Abstract VM!");
    if (argc > 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> [--continue-on-error | --validate-file]\n";
        return 1;
    }

    try
    {
        RunMode mode = parseMode(argc, argv);

        std::unique_ptr<inputReader> input = makeInput(argc, argv);

        RunResult result = runProgram(*input, virtualMachine, mode);

        if (!result.sawExit)
        {
            std::cerr << "No exit instruction found.\nExiting with error.\n";
            return 1;
        }

        if (result.hadErrors)
           return 1;

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
        std::cerr << "Unexpected std::exception: " << e.what() << "\nExiting with error.\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unexpected unknown exception.\nExiting with error.\n";
        return 1;
    }
}

#endif
