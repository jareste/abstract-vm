#include <cmath>
#include <type_traits>
#include <iostream>
#include "Operand.hpp"

/* Private helpers */

template <typename T>
static std::string m_toCanonicalString(T v, eOperandType type)
{
    std::ostringstream oss;

    if (type == Int8 || type == Int16 || type == Int32)
    {
        oss << static_cast<long long>(v);
        return oss.str();
    }

    if (type == Float)
    {
        oss << std::setprecision(std::numeric_limits<float>::max_digits10)
            << static_cast<float>(v);
        return oss.str();
    }

    oss << std::setprecision(std::numeric_limits<double>::max_digits10)
        << static_cast<double>(v);

    return oss.str();
}

template <typename T>
template<typename R>
IOperand const * Operand<T>::makeOp(IOperand const & rhs, char op, eOperandType type) const
{
    R rhsVal;
    if constexpr (std::is_floating_point<R>::value)
        rhsVal = static_cast<R>(std::stod(rhs.toString()));
    else
    {
        long double rhsLD = std::stold(rhs.toString());
        rhsVal = static_cast<R>(rhsLD);
    }

    if ((op == '/' || op == '%') && rhsVal == static_cast<R>(0))
        throw DivisionByZero("Error: Division by zero");

    R lhsVal = static_cast<R>(this->_value);
    R result{};
    switch (op)
    {
        case '+': result = lhsVal + rhsVal; break;
        case '-': result = lhsVal - rhsVal; break;
        case '*': result = lhsVal * rhsVal; break;
        case '/': result = lhsVal / rhsVal; break;
        case '%':
            if constexpr (std::is_floating_point<R>::value)
                result = static_cast<R>(std::fmod(lhsVal, rhsVal));
            else
                result = static_cast<R>(lhsVal % rhsVal);
            break;
        default:
            throw UnknownOperation("Unknown operator in makeOp");
    }
#ifdef DEBUG
    std::cout << "Operation: " << lhsVal << " " << op << " " << rhsVal << " = " << result << std::endl;
#endif
    return new Operand<R>(result, type);
}

template <typename T>
IOperand const * Operand<T>::operate(IOperand const & rhs, char op) const
{
    eOperandType resultType = (this->getPrecision() >= rhs.getPrecision()) ? this->getType() : rhs.getType();

    switch (resultType)
    {
        case Int8: return makeOp<int8_t>(rhs, op, Int8);
        case Int16: return makeOp<int16_t>(rhs, op, Int16);
        case Int32: return makeOp<int32_t>(rhs, op, Int32);
        case Float: return makeOp<float>(rhs, op, Float);
        case Double: return makeOp<double>(rhs, op, Double);
        case None:
            throw InvalidOperandType("Invalid operand type in operation.");
    }
    throw InvalidOperandType("Invalid operand type in operation.");
}

/* Actual methods */

template <typename T>
Operand<T>::Operand(T value, eOperandType type) : _value(value), _type(type), _strValue(m_toCanonicalString(value, type))
{
#ifdef DEBUG
    std::cout << "Operand created: " << _value << " of type " << typeName(_type) << " string: " << _strValue << std::endl;
#endif
}

template <typename T>
Operand<T>::~Operand(void)
{
#ifdef DEBUG2
    std::cout << "Operand destroyed: " << _value << " of type " << typeName(_type) << std::endl;
#endif
}


template <typename T>
int Operand<T>::getPrecision(void) const
{
    return static_cast<int>( _type );
}

template <typename T>
eOperandType Operand<T>::getType(void) const
{
    return _type;
}

template <typename T>
IOperand const * Operand<T>::operator+(IOperand const & rhs) const
{
    return operate(rhs, '+');
}

template <typename T>
IOperand const * Operand<T>::operator-(IOperand const & rhs) const
{
    return operate(rhs, '-');
}

template <typename T>
IOperand const * Operand<T>::operator*(IOperand const & rhs) const
{
    return operate(rhs, '*');
}

template <typename T>
IOperand const * Operand<T>::operator/(IOperand const & rhs) const
{
    return operate(rhs, '/');
}

template <typename T>
IOperand const * Operand<T>::operator%(IOperand const & rhs) const
{
    return operate(rhs, '%');
}

template <typename T>
std::string const & Operand<T>::toString(void) const
{
    return _strValue;
}

template class Operand<int8_t>;
template class Operand<int16_t>;
template class Operand<int32_t>;
template class Operand<float>;
template class Operand<double>;
