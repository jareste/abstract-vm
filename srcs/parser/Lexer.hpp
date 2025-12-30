#pragma once
#include <vector>
#include <string>
#include "InputReader.hpp"
#include "../exception/Exception.hpp"

/* Token kinds
 * Ident  - identifiers (e.g., 'push', 'pop', 'int32', etc.)
 * Number - numeric literals (e.g., '123', '-456', etc.)
 * LParen - left parenthesis '('
 * RParen - right parenthesis ')'
 * End    - end of line/input
 */
enum class TokenKind { Ident, Number, LParen, RParen, End };

struct Token {
    TokenKind kind;
    std::string lexeme;
    size_t line;
    int col;
};

class Lexer {
    private:
        Lexer();
        Lexer(const Lexer& other);
        const Lexer& operator=(const Lexer& other);
        ~Lexer();
    public:
        static std::vector<Token> tokenize(Line const& ln);
};
