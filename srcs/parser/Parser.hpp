#pragma once
#include <string>
#include <optional>
#include <vector>
#include "../operand/IOperand.hpp"
#include "../vm/vm.hpp"
#include "Lexer.hpp"

class Parser
{
    private:
        Parser();
        Parser(const Parser& other);
        const Parser& operator=(const Parser& other);
        ~Parser();

    public:
        static Instruction parseInstruction(const std::vector<Token>& tokens);

};