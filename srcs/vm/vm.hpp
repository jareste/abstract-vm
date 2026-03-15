#pragma once
#include <vector>
#include <string>
#include <optional>
#include "../operand/IOperand.hpp"
#include "../operand/OperandFactory.hpp"

enum class OpCode { Push, Pop, Dump, Assert, Add, Sub, Mul, Div, Mod, Print, Exit, Swap, Clone, Min, Max, Rotate, Root, None };

struct OpValue { eOperandType type; std::string literal; };
struct Instruction {
    int line;
    OpCode op;
    std::optional<OpValue> arg;
};

class vm
{
    private:
        std::vector<std::unique_ptr<IOperand const>> _stack;

        void performOperation(const Instruction& instr);

        vm(const vm& other);
        const vm& operator=(const vm& other);

        void m_executePush(const Instruction& instr);
        void m_executePop(const Instruction& instr);
        void m_executeDump(const Instruction& instr);
        void m_executeAssert(const Instruction& instr);
        void m_executePrint(const Instruction& instr);
        void m_executeSwap(const Instruction& instr);
        void m_executeClone(const Instruction& instr);
        void m_executeRotate(const Instruction& instr);
        void m_executeExit(const Instruction& instr);

    public:
        vm();
        ~vm();
        void executeInstruction(const Instruction& instr);

};


