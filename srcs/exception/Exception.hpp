#pragma once
#include <exception>
#include <string>

class AVMException : public std::exception
{
    private:
        std::string _msg;
    public:
        AVMException() {};
        AVMException(const AVMException& other);
        const AVMException& operator=(const AVMException& other);
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }
        virtual ~AVMException() throw (){}
};

class DivisionByZero : public AVMException, public std::runtime_error
{
    private:
        std::string _msg;
    public:
        DivisionByZero(const std::string& msg) : AVMException(), std::runtime_error(msg), _msg(msg) {}
        DivisionByZero(int lineNumber): AVMException(), std::runtime_error("Division by zero at line " + std::to_string(lineNumber)), _msg("Division by zero at line " + std::to_string(lineNumber))
        {
        }
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~DivisionByZero() throw (){}
}; 

class OverflowException : public AVMException, public std::overflow_error
{
    private:
        std::string _msg;
    public:
        OverflowException(const std::string& msg) : AVMException(), std::overflow_error(msg), _msg(msg) {}
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~OverflowException() throw (){}
};

class UnderflowException : public AVMException, public std::underflow_error
{
    private:
        std::string _msg;
    public:
        UnderflowException(const std::string& msg) : AVMException(), std::underflow_error(msg), _msg(msg) {}
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~UnderflowException() throw (){}
};

class InvalidValue : public AVMException, public std::invalid_argument
{
    private:
        std::string _msg;
    public:
        InvalidValue(const std::string& msg) : AVMException(), std::invalid_argument(msg), _msg(msg) {}
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~InvalidValue() throw (){}
};

class LexicalError : public AVMException
{
    private:
        std::string _msg;
    public:
        LexicalError(int line, int col, const std::string& msg) : AVMException()
        {
            _msg = "Lexical error at line " + std::to_string(line) + ", col " + std::to_string(col) + ": " + msg;
        }
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~LexicalError() throw (){}
};

class SyntaxError : public AVMException
{
    private:
        std::string _msg;
    public:
        SyntaxError(int line, const std::string& msg) : AVMException()
        {
            _msg = "Syntax error at line " + std::to_string(line) + ": " + msg;
        }
        SyntaxError(int line, int col, const std::string& msg) : AVMException()
        {
            _msg = "Syntax error at line " + std::to_string(line) + ", col " + std::to_string(col) + ": " + msg;
        }

        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~SyntaxError() throw (){}
};

class StackUnderflow : public AVMException, public std::underflow_error
{
    private:
        std::string _msg;
    public:
        StackUnderflow(const std::string& msg) : AVMException(), std::underflow_error(msg), _msg(msg) {}
        StackUnderflow(int line, const std::string& msg) : AVMException(), std::underflow_error("Stack underflow at line " + std::to_string(line) + ": " + msg), _msg("Stack underflow at line " + std::to_string(line) + ": " + msg) {}
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~StackUnderflow() throw (){}
};

class MissingExitInstruction : public AVMException
{
    private:
        std::string _msg;
    public:
        MissingExitInstruction() : AVMException()
        {
            _msg = "Missing exit instruction in program.";
        }
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~MissingExitInstruction() throw (){}
};

class AssertionFailed : public AVMException
{
    private:
        std::string _msg;
    public:
        AssertionFailed(int line, const std::string& msg) : AVMException()
        {
            _msg = "Assertion failed at line " + std::to_string(line) + ": " + msg;
        }
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~AssertionFailed() throw (){}
};

class FailedToOpenFile : public AVMException
{
    private:
        std::string _msg;
    public:
        FailedToOpenFile(const std::string& filename) : AVMException()
        {
            _msg = "Failed to open file: " + filename;
        }
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~FailedToOpenFile() throw (){}
};

class InvalidOperandType : public AVMException
{
    private:
        std::string _msg;
    public:
        InvalidOperandType(const std::string& msg) : AVMException(), _msg(msg) {}
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~InvalidOperandType() throw (){}
};

class UnknownOperation : public AVMException
{
    private:
        std::string _msg;
    public:
        UnknownOperation(const std::string& msg) : AVMException(), _msg(msg) {}
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~UnknownOperation() throw (){}
};

class InvalidInstruction : public AVMException
{
    private:
        std::string _msg;
    public:
        InvalidInstruction(int line, const std::string& msg) : AVMException()
        {
            _msg = "Invalid instruction at line " + std::to_string(line) + ": " + msg;
        }
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~InvalidInstruction() throw (){}
};
