#pragma once
#include <vector>
#include <string>
#include "InputReader.hpp"
#include "../exception/Exception.hpp"

enum class TokenKind { Ident, Number, LParen, RParen, End };

struct Token {
    TokenKind kind;
    std::string lexeme;
    size_t line;
    int col;
};

class Lexer {
public:
    static std::vector<Token> tokenize(Line const& ln);
};

