#pragma once
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
#include <cmath>
#include <type_traits>
#include <cassert>
#include "IOperand.hpp"

template <typename T>
class Operand : public IOperand
{
    private:
        const T _value;
        const eOperandType _type;
        eOperandType _precision;
        std::string* _strValue;

        template<typename R>
        IOperand const * makeOp(IOperand const & rhs, char op, eOperandType type) const;
        IOperand const * operate(IOperand const & rhs, char op) const;

    public:
        Operand( T value, eOperandType type );
        virtual ~Operand( void );

        virtual int getPrecision( void ) const override;
        virtual eOperandType getType( void ) const override;
        virtual IOperand const * operator+( IOperand const & rhs ) const override;
        virtual IOperand const * operator-( IOperand const & rhs ) const override;
        virtual IOperand const * operator*( IOperand const & rhs ) const override;
        virtual IOperand const * operator/( IOperand const & rhs ) const override;
        virtual IOperand const * operator%( IOperand const & rhs ) const override;
        virtual std::string const & toString( void ) const override;
};
