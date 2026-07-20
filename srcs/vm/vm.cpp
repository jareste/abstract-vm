#include "vm.hpp"
#include <iostream>
#include <map>
#include <unordered_map>
#include "../exception/Exception.hpp"
#include "../debug_log.hpp"

static std::unordered_map<OpCode, std::string_view> OpCodeNames = {
    {OpCode::Push, "Push"}, {OpCode::Pop, "Pop"}, {OpCode::Dump, "Dump"},
    {OpCode::Assert, "Assert"}, {OpCode::Add, "Add"}, {OpCode::Sub, "Sub"},
    {OpCode::Mul, "Mul"}, {OpCode::Div, "Div"}, {OpCode::Mod, "Mod"},
    {OpCode::Print, "Print"}, {OpCode::Exit, "Exit"}, {OpCode::Swap, "Swap"},
    {OpCode::Clone, "Clone"}, {OpCode::Min, "Min"}, {OpCode::Max, "Max"},
    {OpCode::Rotate, "Rotate"}, {OpCode::Root, "Root"}
};

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
        case OpCode::Swap: std::cout << "Swap"; break;
        case OpCode::Clone: std::cout << "Clone"; break;
        case OpCode::Min: std::cout << "Min"; break;
        case OpCode::Max: std::cout << "Max"; break;
        case OpCode::Rotate: std::cout << "Rotate"; break;
        case OpCode::Root: std::cout << "Root"; break;
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
    std::unique_ptr<IOperand const> op1;
    std::unique_ptr<IOperand const> op2;
    std::unique_ptr<IOperand const> result = nullptr;

    if (_stack.size() < 2)
    {
        throw StackUnderflow(instr.line, "Not enough values on stack for operation " + std::string(OpCodeNames.at(instr.op)));
    }

    op1 = std::move(_stack.top());
    _stack.pop();
    op2 = std::move(_stack.top());
    _stack.pop();

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
        case OpCode::Min:
            result = *op2 < *op1;
            LOG_OP("Min result: " + result->toString(), instr.line);
            break;
        case OpCode::Max:
            result = *op2 > *op1;
            LOG_OP("Max result: " + result->toString(), instr.line);
            break;
        case OpCode::Root:
            result = op2->root(*op1);
            LOG_OP("Root result (placeholder): " + result->toString(), instr.line);
            break;
        default:
            /* never */
            break;
    }

    _stack.push(std::move(result));
}

void vm::m_executeExit(const Instruction& instr)
{
    (void)instr; /* KCH */
    LOG("Executing Exit instruction. Exiting.");
    exit(0);
}

void vm::m_executePush(const Instruction& instr)
{
    LOG("Executing Push instruction with argument: " << instr.arg->literal);
    _stack.push(OperandFactory::createOperand(instr.arg->type, instr.arg->literal));
}

void vm::m_executePop(const Instruction& instr)
{
    LOG("Executing Pop instruction.");
    if (!_stack.empty())
        _stack.pop();
    else
        throw StackUnderflow(instr.line, "Pop on empty stack");
}

void vm::m_executeDump(const Instruction& instr)
{
    std::vector<std::unique_ptr<IOperand const>> tmp;
 
    (void)instr; /* KCH */
    LOG("Executing Dump instruction.");

    tmp.reserve(_stack.size());

    while (!_stack.empty())
    {
        tmp.push_back(std::move(_stack.top()));
        _stack.pop();
    }

    for (auto it = tmp.begin(); it != tmp.end(); ++it)
        std::cout << (*it)->toString() << std::endl;

    for (auto it = tmp.rbegin(); it != tmp.rend(); ++it)
        _stack.push(std::move(*it));
}

void vm::m_executeAssert(const Instruction& instr)
{
    std::unique_ptr<IOperand const> op1;

    LOG("Executing Assert instruction.");
    if (!_stack.empty())
    {
        op1 = std::move(_stack.top());
        _stack.pop();
        std::unique_ptr<IOperand const> expected(OperandFactory::createOperand(instr.arg->type, instr.arg->literal));
        if (op1->getType() != expected->getType() || op1->toString() != expected->toString())
            throw AssertionFailed(instr.line, "Assertion failed");

        _stack.push(std::move(op1));
    }
    else
    {
        throw StackUnderflow(instr.line, "Assert on empty stack");
    }
}

void vm::m_executePrint(const Instruction& instr)
{
    LOG("Executing Print instruction.");
    if (_stack.empty())
        throw StackUnderflow(instr.line, "Print on empty stack");

    {
        std::unique_ptr<IOperand const> top = std::move(_stack.top());
        _stack.pop();
        if (top->getType() != Int8)
            throw AssertionFailed(instr.line, "Print instruction requires top of stack to be Int8");

        char c = static_cast<char>(std::stoi(top->toString()));
        std::cout << c << std::endl;
        _stack.push(std::move(top));
    }
}

void vm::m_executeSwap(const Instruction& instr)
{
    LOG("Executing Swap instruction.");
    if (_stack.size() < 2)
        throw StackUnderflow(instr.line, "Not enough values on stack to swap");

    {
        std::unique_ptr<IOperand const> top1 = std::move(_stack.top());
        _stack.pop();
        std::unique_ptr<IOperand const> top2 = std::move(_stack.top());
        _stack.pop();

        _stack.push(std::move(top1));
        _stack.push(std::move(top2));
    }
}

void vm::m_executeClone(const Instruction& instr)
{
    LOG("Executing Clone instruction.");
    if (_stack.empty())
        throw StackUnderflow(instr.line, "Clone on empty stack");
    {
        std::unique_ptr<IOperand const> top = std::move(_stack.top());
        _stack.pop();
        _stack.push(top->clone());
        _stack.push(std::move(top));
    }
}

void vm::m_executeRotate(const Instruction& instr)
{
    LOG("Executing Rotate instruction.");
    if (_stack.empty())
        throw StackUnderflow(instr.line, "Rotate on empty stack");
    {
        std::vector<std::unique_ptr<IOperand const>> stack_copy;
        stack_copy.reserve(_stack.size());

        while (!_stack.empty())
        {
            stack_copy.push_back(std::move(_stack.top()));
            _stack.pop();
        }
    
        std::reverse(stack_copy.begin(), stack_copy.end());

        auto stack_size = stack_copy.size();
        for (size_t i = 0; i < stack_size; ++i)
            _stack.push(std::move(stack_copy[(i + 1) % stack_size]));
    }
}

void vm::executeInstruction(const Instruction& instr)
{
    static std::unordered_map<OpCode, void (vm::*)(const Instruction&)> opcodeHandlers = {
        {OpCode::Push, &vm::m_executePush},
        {OpCode::Pop, &vm::m_executePop},
        {OpCode::Dump, &vm::m_executeDump},
        {OpCode::Assert, &vm::m_executeAssert},
        {OpCode::Add, &vm::performOperation},
        {OpCode::Sub, &vm::performOperation},
        {OpCode::Mul, &vm::performOperation},
        {OpCode::Div, &vm::performOperation},
        {OpCode::Mod, &vm::performOperation},
        {OpCode::Print, &vm::m_executePrint},
        {OpCode::Exit, &vm::m_executeExit},
        {OpCode::Swap, &vm::m_executeSwap},
        {OpCode::Clone, &vm::m_executeClone},
        {OpCode::Min, &vm::performOperation},
        {OpCode::Max, &vm::performOperation},
        {OpCode::Rotate, &vm::m_executeRotate},
        {OpCode::Root, &vm::performOperation}
    };

    LOG("Stack size before instruction: " << _stack.size());
    m_print_instruction(instr);
    auto handlerIt = opcodeHandlers.find(instr.op);
    if (handlerIt != opcodeHandlers.end())
    {
        auto handler = handlerIt->second;
        (this->*handler)(instr);
    }
    else
    {
        LOG("No handler for opcode. Skipping instruction.");
        throw InvalidInstruction(instr.line, "Invalid opcode");
    }

    LOG("Stack size after instruction: " << _stack.size());
}

vm::vm()
{
}

vm::~vm()
{
}
