#include <unordered_map>
#include <string_view>
#include "Parser.hpp"
#include "../exception/Exception.hpp"
#include <iostream>
// #define PRINT_TOKENS

static bool m_isValidFloatLiteral(const std::string& s)
{
    bool hasDigitsAfter;
    bool hasDigitsBefore;
    size_t i = 0;

    if (s[i] == '-') i++;

    hasDigitsBefore = false;
    while (i < s.size() && isdigit(s[i]))
    {
        hasDigitsBefore = true;
        i++;
    }

    if (!hasDigitsBefore || i >= s.size() || s[i] != '.')
        return false;

    i++;

    hasDigitsAfter = false;
    while (i < s.size() && isdigit(s[i]))
    {
        hasDigitsAfter = true;
        i++;
    }

    return hasDigitsAfter && (i == s.size());
}

Instruction Parser::parseInstruction(const std::vector<Token>& tokens)
{
    Instruction instr;
    bool insideParens = false;
    eOperandType argType = None;
    
    static const std::unordered_map<std::string_view, OpCode> opMap = {
        {"push", OpCode::Push}, {"pop", OpCode::Pop}, {"dump", OpCode::Dump},
        {"assert", OpCode::Assert}, {"add", OpCode::Add}, {"sub", OpCode::Sub},
        {"mul", OpCode::Mul}, {"div", OpCode::Div}, {"mod", OpCode::Mod},
        {"print", OpCode::Print}, {"exit", OpCode::Exit}
    };

    static const std::unordered_map<std::string_view, eOperandType> typeMap = {
        {"int8", Int8}, {"int16", Int16}, {"int32", Int32},
        {"float", Float}, {"double", Double}
    };

    instr.op = OpCode::None;
    instr.line = tokens.empty() ? 0 : tokens[0].line;

    for (const auto& token : tokens)
    {
#ifdef PRINT_TOKENS
        std::cout << "  Kind: ";
        switch (token.kind)
        {
            case TokenKind::Ident: std::cout << "Ident"; break;
            case TokenKind::Number: std::cout << "Number"; break;
            case TokenKind::LParen: std::cout << "LParen"; break;
            case TokenKind::RParen: std::cout << "RParen"; break;
            case TokenKind::End: std::cout << "End"; break;
        }
        std::cout << ", Lexeme: '" << token.lexeme << "', Line: " << token.line << ", Col: " << token.col << std::endl;
#endif

        switch (token.kind)
        {
        case TokenKind::Ident:
        {
            std::string_view lex = token.lexeme;
            auto opIt = opMap.find(lex);
            if (opIt != opMap.end())
            {
                if (instr.op != OpCode::None)
                    throw SyntaxError(token.line, token.col, "Duplicate instruction/opcode: " + token.lexeme);
                instr.op = opIt->second;
            }
            else
            {
                auto typeIt = typeMap.find(lex);
                if (typeIt != typeMap.end())
                {
                    if (argType != None)
                        throw SyntaxError(token.line, token.col, "Duplicate type specifier: " + token.lexeme);
                    argType = typeIt->second;
                }
                else
                {
                    throw SyntaxError(token.line, token.col, "Unknown identifier: " + token.lexeme);
                }
            }
            break;
        }
        case TokenKind::Number:
        {
            if (!insideParens)
            {
                throw SyntaxError(token.line, token.col, "Unexpected number token outside parentheses: " + token.lexeme);
            }

            if (argType == None)
                throw SyntaxError(token.line, token.col, "Missing type specifier for value: " + token.lexeme);

            if (argType == Float || argType == Double)
            {
                if (!m_isValidFloatLiteral(token.lexeme))
                    throw SyntaxError(token.line, token.col, "Invalid float/double literal: " + token.lexeme);
            }

            instr.arg = OpValue{argType, token.lexeme};
            break;
        }
        case TokenKind::LParen:
            insideParens = true;
            break;
        case TokenKind::RParen:
            insideParens = false;
            break;
        case TokenKind::End:
            if (insideParens)
                throw SyntaxError(token.line, token.col, "Unexpected end of line inside parentheses");

            return instr;
        }

    } 

    /* Should never happen as 'End' should always be included. */
    return instr;
}
