#pragma once
#include <vector>
#include <string>
#include <optional>
#include "../operand/IOperand.hpp"
#include "../operand/OperandFactory.hpp"

enum class OpCode { Push, Pop, Dump, Assert, Add, Sub, Mul, Div, Mod, Print, Exit, None };

struct OpValue { eOperandType type; std::string literal; };
struct Instruction {
    int line;
    OpCode op;
    std::optional<OpValue> arg;
};

class vm
{
    private:
        std::vector<IOperand const *> _stack;

        void performOperation(const Instruction& instr);

        vm(const vm& other);
        const vm& operator=(const vm& other);

    public:
        vm();
        ~vm();
        void executeInstruction(const Instruction& instr);

};


