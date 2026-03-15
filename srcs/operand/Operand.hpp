#pragma once
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
#include <cmath>
#include <type_traits>
#include <cassert>
#include "IOperand.hpp"
#include "../exception/Exception.hpp"

template <typename T>
class Operand : public IOperand
{
    private:
        const T _value;
        const eOperandType _type;
        std::string _strValue;

        template<typename R>
        std::unique_ptr<IOperand const> makeOp(const IOperand& rhs, char op, eOperandType type) const;
        std::unique_ptr<IOperand const> operate(const IOperand& rhs, char op) const;


        Operand();
        Operand(Operand const& other);
        Operand const& operator=(Operand const& other);

    public:
        Operand( T value, eOperandType type );
        virtual ~Operand( void );

        virtual std::unique_ptr<IOperand const> clone( void ) const override;
        virtual int getPrecision( void ) const override;
        virtual eOperandType getType( void ) const override;
        // virtual IOperand const * operator+( IOperand const & rhs ) const override;
        virtual std::unique_ptr<IOperand const> operator+(IOperand const& rhs) const override;
        virtual std::unique_ptr<IOperand const> operator-(IOperand const& rhs) const override;
        virtual std::unique_ptr<IOperand const> operator*(IOperand const& rhs) const override;
        virtual std::unique_ptr<IOperand const> operator/(IOperand const& rhs) const override;
        virtual std::unique_ptr<IOperand const> operator%(IOperand const& rhs) const override;
        virtual std::unique_ptr<IOperand const> operator<(IOperand const& rhs) const override;
        virtual std::unique_ptr<IOperand const> operator>(IOperand const& rhs) const override;
        virtual std::unique_ptr<IOperand const> root(IOperand const& rhs) const override;
        virtual std::string const & toString( void ) const override;
};
