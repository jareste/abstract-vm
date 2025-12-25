#pragma once
#include <string>
#include "IOperand.hpp"

class OperandFactory {
public:
    static IOperand const* createOperand(eOperandType type, std::string const& value);

private:
    OperandFactory();
    OperandFactory(const OperandFactory& other);
    const OperandFactory& operator=(const OperandFactory& other);
    ~OperandFactory();

    typedef IOperand const* (OperandFactory::*CreateFn)(std::string const&) const;

    IOperand const* createInt8(std::string const& value) const;
    IOperand const* createInt16(std::string const& value) const;
    IOperand const* createInt32(std::string const& value) const;
    IOperand const* createFloat(std::string const& value) const;
    IOperand const* createDouble(std::string const& value) const;
};
