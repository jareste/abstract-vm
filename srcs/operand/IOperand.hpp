#pragma once
#include <string>
#include <array>

typedef enum eOperandType
{
    Int8 = 0,
    Int16,
    Int32,
    Float,
    Double
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
    }
    return "Unknown";
}

class IOperand
{
public:
virtual int getPrecision( void ) const = 0; // Precision of the type of the instance
virtual eOperandType getType( void ) const = 0; // Type of the instance
virtual IOperand const * operator+( IOperand const & rhs ) const = 0; // Sum
virtual IOperand const * operator-( IOperand const & rhs ) const = 0; // Difference
virtual IOperand const * operator*( IOperand const & rhs ) const = 0; // Product
virtual IOperand const * operator/( IOperand const & rhs ) const = 0; // Quotient
virtual IOperand const * operator%( IOperand const & rhs ) const = 0; // Modulo
virtual std::string const & toString( void ) const = 0; // String representation of the instance
virtual ~IOperand( void ) {}
};
