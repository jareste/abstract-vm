#pragma once
#include <string>
#include <array>
#include <memory>

typedef enum eOperandType
{
    Int8 = 0,
    Int16,
    Int32,
    Float,
    Double,
    None
} eOperandType;

inline const char* typeName(eOperandType t)
{
    switch(t)
    {
        case Int8: return "Int8";
        case Int16: return "Int16";
        case Int32: return "Int32";
        case Float: return "Float";
        case Double: return "Double";
        case None: return "None";
    }
    return "Unknown";
}

class IOperand
{
public:
virtual int getPrecision( void ) const = 0; // Precision of the type of the instance
virtual eOperandType getType( void ) const = 0; // Type of the instance
// virtual std::unique_ptr<IOperand const> operator+( IOperand const & rhs ) const = 0; // Sum
virtual std::unique_ptr<IOperand const> operator+(IOperand const& rhs) const = 0; // Sum
virtual std::unique_ptr<IOperand const> operator-(IOperand const& rhs) const = 0; // Difference
virtual std::unique_ptr<IOperand const> operator*(IOperand const& rhs) const = 0; // Product
virtual std::unique_ptr<IOperand const> operator/(IOperand const& rhs) const = 0; // Quotient
virtual std::unique_ptr<IOperand const> operator%(IOperand const& rhs) const = 0; // Modulo
virtual std::unique_ptr<IOperand const> operator<(IOperand const& rhs) const = 0; // Min
virtual std::unique_ptr<IOperand const> operator>(IOperand const& rhs) const = 0; // Max
virtual std::unique_ptr<IOperand const> root(IOperand const& rhs) const = 0;
virtual std::string const & toString( void ) const = 0; // String representation of the instance

virtual std::unique_ptr<IOperand const> clone( void ) const = 0; // Clone the instance
virtual ~IOperand( void ) {}
};
