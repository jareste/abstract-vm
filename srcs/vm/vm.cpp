#include "vm.hpp"
#include <iostream>
#include "../exception/Exception.hpp"
#include "../debug_log.hpp"

void m_print_instruction(const Instruction& instr)
{
#ifdef PRINT_PARSED_INSTRUCTIONS
    std::cout << "Instruction at line " << instr.line << ": opcode ";
    switch (instr.op)
    {
        case OpCode::Push: std::cout << "Push"; break;
        case OpCode::Pop: std::cout << "Pop"; break;
        case OpCode::Dump: std::cout << "Dump"; break;
        case OpCode::Assert: std::cout << "Assert"; break;
        case OpCode::Add: std::cout << "Add"; break;
        case OpCode::Sub: std::cout << "Sub"; break;
        case OpCode::Mul: std::cout << "Mul"; break;
        case OpCode::Div: std::cout << "Div"; break;
        case OpCode::Mod: std::cout << "Mod"; break;
        case OpCode::Print: std::cout << "Print"; break;
        case OpCode::Exit: std::cout << "Exit"; break;
        default: std::cout << "Unknown"; break;
    }
    if (instr.arg.has_value())
    {
        std::cout << ", argument type: " << typeName(instr.arg->type);
        std::cout << ", literal: " << instr.arg->literal;
    }
    std::cout << std::endl;
#else
    (void)instr; // KCH
#endif
}


void vm::performOperation(const Instruction& instr)
{
    IOperand const* op1;
    IOperand const* op2;
    IOperand const* result = nullptr;

    if (_stack.size() < 2)
    {
        throw StackUnderflow(instr.line, "Not enough values on stack for operation");
    }

    op1 = dynamic_cast<IOperand const*>(_stack.back());
    _stack.pop_back();
    op2 = dynamic_cast<IOperand const*>(_stack.back());
    _stack.pop_back();

    switch (instr.op)
    {
        case OpCode::Add:
            result = *op2 + *op1;
            LOG_OP("Add result: " + result->toString(), instr.line);
            break;
        case OpCode::Sub:
            result = *op2 - *op1;
            LOG_OP("Sub result: " + result->toString(), instr.line);
            break;
        case OpCode::Mul:
            result = *op2 * *op1;
            LOG_OP("Mul result: " + result->toString(), instr.line);
            break;
        case OpCode::Div:
            result = *op2 / *op1;
            LOG_OP("Div result: " + result->toString(), instr.line);
            break;
        case OpCode::Mod:
            result = *op2 % *op1;
            LOG_OP("Mod result: " + result->toString(), instr.line);
            break;
        default:
            /* never */
            break;
    }

    _stack.push_back(result);
    delete op1;
    delete op2;
}

void vm::executeInstruction(const Instruction& instr)
{
    IOperand const* op1;

    m_print_instruction(instr);
    switch (instr.op)
    {
        case OpCode::Push:
            LOG("Executing Push instruction.");
            _stack.push_back(OperandFactory::createOperand(instr.arg->type, instr.arg->literal));
            break;
        case OpCode::Pop:
            LOG("Executing Pop instruction.");
            if (!_stack.empty())
            {
                delete _stack.back();
                _stack.pop_back();
            }
            else
                throw StackUnderflow(instr.line, "Pop on empty stack");
            break;
        case OpCode::Dump:
            LOG("Executing Dump instruction.");
            for (auto it = _stack.rbegin(); it != _stack.rend(); ++it)
                std::cout << (*it)->toString() << std::endl;

            break;
        case OpCode::Assert:
            LOG("Executing Assert instruction.");
            op1 = dynamic_cast<IOperand const*>(_stack.back());
            {
                IOperand const* expected = OperandFactory::createOperand(instr.arg->type, instr.arg->literal);
                if (op1->getType() != expected->getType() || op1->toString() != expected->toString())
                {
                    delete expected;
                    throw AssertionFailed(instr.line, "Assertion failed");
                }
                delete expected;
            }
            break;
        case OpCode::Add:
            LOG("Executing Add instruction.");
            this->performOperation(instr);
            break;
        case OpCode::Sub:
            LOG("Executing Sub instruction.");
            this->performOperation(instr);
            break;
        case OpCode::Mul:
            LOG("Executing Mul instruction.");
            this->performOperation(instr);
            break;
        case OpCode::Div:
            LOG("Executing Div instruction.");
            this->performOperation(instr);
            break;
        case OpCode::Mod:
            LOG("Executing Mod instruction.");
            this->performOperation(instr);
            break;
        case OpCode::Print:
            LOG("Executing Print instruction.");
            if (_stack.empty())
                throw StackUnderflow(instr.line, "Print on empty stack");

            {
                IOperand const* top = _stack.back();
                if (top->getType() != Int8)
                {
                    throw AssertionFailed(instr.line, "Print instruction requires top of stack to be Int8");
                }
                char c = static_cast<char>(std::stoi(top->toString()));
                std::cout << c << std::endl;
            }
            break;
        case OpCode::Exit:
            LOG("Executing Exit instruction.");
            exit(0);
            break;
        default:
            LOG("Unknown instruction.");
            break;
    }
}

vm::vm()
{
}

vm::~vm()
{
    for (auto operand : _stack)
    {
        delete operand;
    }
}
