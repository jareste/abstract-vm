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

class DivisionByZero : public AVMException
{
    private:
        std::string _msg;
    public:
        DivisionByZero(const std::string& msg) : AVMException(), _msg(msg) {}
        DivisionByZero(int lineNumber): AVMException()
        {
            _msg = "Division by zero at line " + std::to_string(lineNumber);
        }
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~DivisionByZero() throw (){}
}; 

class OverflowException : public AVMException
{
    private:
        std::string _msg;
    public:
        OverflowException(const std::string& msg) : AVMException(), _msg(msg) {}
        virtual const char* what() const throw()
        {
            return _msg.c_str();
        }

        virtual ~OverflowException() throw (){}
};

