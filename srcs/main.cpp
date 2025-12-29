#include <iostream>
#include <fstream>
#include <memory>
#include "operand/Operand.hpp"
#include "parser/InputReader.hpp"
#include "parser/Lexer.hpp"

#if !defined(TEST_OPERAND_MAIN)
int main(int argc, char** argv)
{
    bool isStdin;
    std::string filename;
    size_t linesRead;

    std::cout << "Hello, Abstract VM!" << std::endl;

    isStdin = (argc == 1);
    filename = (isStdin) ? "stdin" : argv[1];

    inputReader iput(filename, isStdin);
    linesRead = iput.readProgram(1000);
    
    std::cout << "Read " << linesRead << " lines from input." << std::endl;

    Line line;
    line = iput.getLine();
    std::cout << "Printing lines read:" << std::endl;
    while ((line.no != 0))
    {
        std::cout << "Line ";
        std::cout << line.no << ": " << line.text << std::endl;
        std::vector<Token> tokens = Lexer::tokenize(line);

#ifdef PRINT_TOKENS
        std::cout << "Tokens:" << std::endl;
        for (const auto& token : tokens) {
            std::cout << "  Kind: ";
            switch (token.kind) {
                case TokenKind::Ident: std::cout << "Ident"; break;
                case TokenKind::Number: std::cout << "Number"; break;
                case TokenKind::LParen: std::cout << "LParen"; break;
                case TokenKind::RParen: std::cout << "RParen"; break;
                case TokenKind::End: std::cout << "End"; break;
            }
            std::cout << ", Lexeme: '" << token.lexeme << "', Line: " << token.line << ", Col: " << token.col << std::endl;
        }
#endif // PRINT_TOKENS

        line = iput.getLine();
    }
    std::cout << "End of lines." << std::endl;

    return 0;
}
#endif
