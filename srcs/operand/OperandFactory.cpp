#include <cmath>
#include <type_traits>
#include <iostream>
#include <charconv>
#include "Operand.hpp"
#include "OperandFactory.hpp"

OperandFactory::OperandFactory() {}
OperandFactory::OperandFactory(const OperandFactory& other) { (void)other; }
const OperandFactory& OperandFactory::operator=(const OperandFactory& other) { (void)other; return *this; }
OperandFactory::~OperandFactory() {}

static long long parseIntStrict(std::string const& s)
{
    if (s.empty())
        throw InvalidValue("Invalid integer literal: " + s);

    long long v = 0;
    auto first = s.data();
    auto last = s.data() + s.size();
    auto res = std::from_chars(first, last, v);

    if (res.ec == std::errc::result_out_of_range)
        throw InvalidValue("Integer out of range: " + s);
    if (res.ec != std::errc() || res.ptr != last)
        throw InvalidValue("Invalid integer literal: " + s);

    return v;
}

static long double parseFloatStrict(std::string const& s)
{
    size_t idx = 0;
    long double v = std::stold(s, &idx);
    if (idx != s.size())
        throw InvalidValue("Invalid float literal: " + s);
    return v;
}

IOperand const* OperandFactory::createOperand(eOperandType type, std::string const& value)
{
    OperandFactory factory;
    const OperandFactory::CreateFn _creators[5] = {
        &OperandFactory::createInt8,
        &OperandFactory::createInt16,
        &OperandFactory::createInt32,
        &OperandFactory::createFloat,
        &OperandFactory::createDouble
    };
    
    if (type < Int8 || type > Double) /* cannot happen. */
        throw InvalidOperandType("Invalid operand type.");

    return (factory.*_creators[type])(value);
}

IOperand const* OperandFactory::createInt8(std::string const& value) const
{
    long long v = parseIntStrict(value);
    if (v < std::numeric_limits<int8_t>::min())
        throw UnderflowException("Int8 underflow: " + value);
    if (v > std::numeric_limits<int8_t>::max())
        throw OverflowException("Int8 overflow: " + value);
    return new Operand<int8_t>(static_cast<int8_t>(v), Int8);
}

IOperand const* OperandFactory::createInt16(std::string const& value) const
{
    long long v = parseIntStrict(value);
    if (v < std::numeric_limits<int16_t>::min())
        throw UnderflowException("Int16 underflow: " + value);
    if (v > std::numeric_limits<int16_t>::max())
        throw OverflowException("Int16 overflow: " + value);
    return new Operand<int16_t>(static_cast<int16_t>(v), Int16);

}

IOperand const* OperandFactory::createInt32(std::string const& value) const
{
    long long v = parseIntStrict(value);
    if (v < std::numeric_limits<int32_t>::min())
        throw UnderflowException("Int32 underflow: " + value);
    if (v > std::numeric_limits<int32_t>::max())
        throw OverflowException("Int32 overflow: " + value);
    return new Operand<int32_t>(static_cast<int32_t>(v), Int32);

}

IOperand const* OperandFactory::createFloat(std::string const& value) const
{
    long double v = parseFloatStrict(value);
    if (v < -std::numeric_limits<float>::max())
        throw UnderflowException("Float underflow: " + value);
    if (v > std::numeric_limits<float>::max())
        throw OverflowException("Float overflow: " + value);
    return new Operand<float>(static_cast<float>(v), Float);
}

IOperand const* OperandFactory::createDouble(std::string const& value) const
{
    long double v = parseFloatStrict(value);
    if (v < -std::numeric_limits<double>::max())
        throw UnderflowException("Double underflow: " + value);
    if (v > std::numeric_limits<double>::max())
        throw OverflowException("Double overflow: " + value);
    return new Operand<double>(static_cast<double>(v), Double);
}
