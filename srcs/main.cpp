#include <iostream>
#include "operand/Operand.hpp"

int main()
{
    std::cout << "Hello, Abstract VM!" << std::endl;
    Operand<int> op(42, Int32);
    Operand<double> op2(3.14, Double);

    IOperand const * result = op + op2;
    std::cout << "Result: " << result->toString() << std::endl;

    delete result;
    return 0;
}
