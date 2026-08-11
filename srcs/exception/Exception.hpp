#pragma once
#include <exception>
#include <string>

class AVMException
{
private:
    std::string _msg;
public:
    explicit AVMException(const std::string& msg)
        : _msg(msg)
    {
    }
    virtual ~AVMException() = default;

    virtual const char* what() const noexcept
    {
        return _msg.c_str();
    }
};

class DivisionByZero : public AVMException
{
public:
    DivisionByZero(const std::string& msg) : AVMException(msg) {}
    DivisionByZero(int lineNumber): AVMException("Division by zero at line " + std::to_string(lineNumber)) {}
}; 

class OverflowException : public AVMException
{
public:
    OverflowException() : AVMException("Overflow exception") {}
    OverflowException(int lineNumber): AVMException("Overflow exception at line " + std::to_string(lineNumber)) {}
    OverflowException(const std::string& msg) : AVMException(msg) {}
};

class UnderflowException : public AVMException
{
public:
    UnderflowException(const std::string& msg) : AVMException(msg) {}
};

class InvalidValue : public AVMException
{
public:
    InvalidValue(const std::string& msg) : AVMException(msg) {}
};

class LexicalError : public AVMException
{
public:
    LexicalError(int line, int col, const std::string& msg) : AVMException("Lexical error at line " + std::to_string(line) + ", col " + std::to_string(col) + ": " + msg) {}
};

class SyntaxError : public AVMException
{
public:
    SyntaxError(int line, const std::string& msg) : AVMException("Syntax error at line " + std::to_string(line) + ": " + msg) {}
    SyntaxError(int line, int col, const std::string& msg) : AVMException("Syntax error at line " + std::to_string(line) + ", col " + std::to_string(col) + ": " + msg) {}
};

class StackUnderflow : public AVMException
{
public:
    StackUnderflow(const std::string& msg) : AVMException(msg) {}
    StackUnderflow(int line, const std::string& msg) : AVMException("Stack underflow at line " + std::to_string(line) + ": " + msg) {}
};

class MissingExitInstruction : public AVMException
{
public:
    MissingExitInstruction() : AVMException("Missing exit instruction in program.") {}
};

class AssertionFailed : public AVMException
{
public:
    AssertionFailed(int line, const std::string& msg) : AVMException("Assertion failed at line " + std::to_string(line) + ": " + msg) {}
};

class FailedToOpenFile : public AVMException
{
public:
    FailedToOpenFile(const std::string& filename) : AVMException("Failed to open file: " + filename) {}
};

class InvalidOperandType : public AVMException
{
public:
    InvalidOperandType(const std::string& msg) : AVMException(msg) {}
};

class UnknownOperation : public AVMException
{
    public:
        UnknownOperation(const std::string& msg) : AVMException(msg) {}
};

class InvalidInstruction : public AVMException
{
    public:
        InvalidInstruction(int line, const std::string& msg) : AVMException("Invalid instruction at line " + std::to_string(line) + ": " + msg) {}
};

class InvalidArgument : public AVMException
{
    public:
        InvalidArgument(const std::string& msg) : AVMException(msg) {}
};
