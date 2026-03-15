#pragma once
#include <string>
#include "IOperand.hpp"

class OperandFactory {
public:
    static std::unique_ptr<IOperand const> createOperand(eOperandType type, std::string const& value);

private:
    OperandFactory();
    OperandFactory(const OperandFactory& other);
    const OperandFactory& operator=(const OperandFactory& other);
    ~OperandFactory();

    typedef std::unique_ptr<IOperand const> (OperandFactory::*CreateFn)(std::string const&) const;

    std::unique_ptr<IOperand const>  createInt8(std::string const& value) const;
    std::unique_ptr<IOperand const>  createInt16(std::string const& value) const;
    std::unique_ptr<IOperand const>  createInt32(std::string const& value) const;
    std::unique_ptr<IOperand const>  createFloat(std::string const& value) const;
    std::unique_ptr<IOperand const>  createDouble(std::string const& value) const;
};
