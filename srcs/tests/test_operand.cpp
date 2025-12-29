#include <iostream>
#include <limits>
#include <string>
#include <exception>

#include "../operand/Operand.hpp"
#include "../operand/OperandFactory.hpp"
#include "../parser/InputReader.hpp"

#if defined(TEST_OPERAND_MAIN)
static void banner(const std::string& name) {
    std::cout << "\n========== " << name << " ==========\n";
}

template <typename Fn>
static void run_case(const std::string& name, Fn&& fn, bool exception = false) {
    const char* PASS_STR = "\033[32m[PASS]\033[0m";
    const char* FAIL_STR = "\033[31m[FAIL]\033[0m";

    std::cout << "\n-- " << name << " --\n";
    try {
        fn();
        if (exception)
        {
            std::cout << FAIL_STR << " Expected exception but none thrown\n";
        }
        else
        {
            std::cout << PASS_STR << " Completed without exception\n";
        }
    } catch (const std::exception& e) {
        if (exception) {
            std::cout << PASS_STR << " Caught expected exception: " << e.what() << "\n";
        }
        else {
            std::cout << FAIL_STR << " Unexpected exception: " << e.what() << "\n";
        }
    } catch (...) {
        if (exception) {
            std::cout << PASS_STR << " Caught expected unknown exception\n";
        }
        else {
            std::cout << FAIL_STR << " Caught unexpected unknown exception\n";
        }
    }
}

static void print_result(const IOperand* r) {
#define PRINT_RESULT
#ifdef PRINT_RESULT
    if (!r) {
        std::cout << "Result: <null>\n";
        return;
    }
    std::cout << "Result: " << r->toString()
              << " (type=" << typeName(r->getType())
              << ", precision=" << typeName(static_cast<eOperandType>(r->getPrecision())) << ")\n";
#else
    (void)r;
#endif
}

int main() {
    std::cout << "AbstractVM Operand torture tests\n";

    banner("1) Basic ops + promotion (precision)");
    run_case("Int32 + Double => Double", []{
        Operand<int32_t> a(42, Int32);
        Operand<double>  b(3.14, Double);
        const IOperand* r = a + b;
        print_result(r);
        delete r;
    });

    run_case("Int8 + Int16 => Int16", []{
        Operand<int8_t>  a(10, Int8);
        Operand<int16_t> b(200, Int16);
        const IOperand* r = a + b;
        print_result(r);
        delete r;
    });

    run_case("Int32 * Float => Float", []{
        Operand<int32_t> a(3, Int32);
        Operand<float>   b(2.5f, Float);
        const IOperand* r = a * b;
        print_result(r);
        delete r;
    });

    banner("2) Non-commutative correctness (sub/div/mod order)");
    run_case("Sub: 10 - 3 = 7", []{
        Operand<int32_t> a(10, Int32);
        Operand<int32_t> b(3, Int32);
        const IOperand* r = a - b;
        print_result(r);
        delete r;
    });

    run_case("Div: 10 / 4 = 2 (int div) o 2.5 (float promotion)", []{
        Operand<int32_t> a(10, Int32);
        Operand<int32_t> b(4, Int32);
        const IOperand* r = a / b;
        print_result(r);
        delete r;

        Operand<int32_t> c(10, Int32);
        Operand<float>   d(4.0f, Float);
        const IOperand* r2 = c / d;
        print_result(r2);
        delete r2;
    });

    banner("3) Division/mod by zero should raise");
    run_case("Div by 0 (int): expect exception", []{
        Operand<int32_t> a(10, Int32);
        Operand<int32_t> b(0, Int32);
        const IOperand* r = a / b;
        print_result(r);
        delete r;
    }, true);

    run_case("Mod by 0 (int): expect exception", []{
        Operand<int32_t> a(10, Int32);
        Operand<int32_t> b(0, Int32);
        const IOperand* r = a % b;
        print_result(r);
        delete r;
    }, true);

    run_case("Div by 0 (float): expect exception", []{
        Operand<float> a(10.0f, Float);
        Operand<float> b(0.0f, Float);
        const IOperand* r = a / b;
        print_result(r);
        delete r;
    }, true);

    banner("4) Mod with floating point (policy-dependent, but must be safe)");
    run_case("Mod Float % Float ", []{
        Operand<float> a(5.5f, Float);
        Operand<float> b(2.0f, Float);
        const IOperand* r = a % b;
        print_result(r);
        delete r;
    });

    run_case("Mod Double % Int32", []{
        Operand<double> a(5.5, Double);
        Operand<int32_t> b(2, Int32);
        const IOperand* r = a % b;
        print_result(r);
        delete r;
    });

    banner("5) Overflow / Underflow construction (should raise)"); 
    run_case("Int8 overflow: 128", []{
        Operand<int8_t> a(static_cast<int8_t>(128), Int8);
        std::cout << "Created: " << a.toString() << "\n";
    });

    run_case("Int8 underflow: -129", []{
        Operand<int8_t> a(static_cast<int8_t>(-129), Int8);
        std::cout << "Created: " << a.toString() << "\n";
    });

    run_case("Float overflow: huge value", []{
        Operand<float> a(std::numeric_limits<float>::max(), Float);
        Operand<float> b(std::numeric_limits<float>::max(), Float);
        const IOperand* r = a + b;
        print_result(r);
        delete r;
    });

    banner("6) Stress: many allocations/deallocations (leaks/double-free/UB)");
    run_case("Repeat 10000 times add promotion Int32+Double", []{
        for (int i = 0; i < 10000; ++i) {
            Operand<int32_t> a(42, Int32);
            Operand<double>  b(3.14, Double);
            const IOperand* r = a + b;
            (void)r->toString(); /* Just to check any unvalid reference or w/e */
            delete r;
        }
        std::cout << "Loop done.\n";
    });

    banner("7) Operand factory creation tests");
    run_case("Create Int8 operand with value 42", []{
        const IOperand* op = OperandFactory::createOperand(Int8, "42");
        print_result(op);
        delete op;
    });

    run_case("Create Float operand with value 3.14", []{
        const IOperand* op = OperandFactory::createOperand(Float, "3.14");
        print_result(op);
        delete op;
    });

    run_case("Create Double operand with value 2.71828", []{
        const IOperand* op = OperandFactory::createOperand(Double, "2.71828");
        print_result(op);
        delete op;
    });

    run_case("Create Int16 operand with overflow value 40000: expect exception", []{
        const IOperand* op = OperandFactory::createOperand(Int16, "40000");
        print_result(op);
    }, true);

    run_case("Create Int32 operand with underflow value -3000000000: expect exception", []{
        const IOperand* op = OperandFactory::createOperand(Int32, "-3000000000");
        print_result(op);
    }, true);

    run_case("Create Float operand with overflow value 1e40: expect exception", []{
        const IOperand* op = OperandFactory::createOperand(Float, "1e40");
        print_result(op);
    }, true);

    run_case("Create Double operand with underflow value -1e400: expect exception", []{
        const IOperand* op = OperandFactory::createOperand(Double, "-1e400");
        print_result(op);
    }, true);

    run_case("Create operand with invalid type: expect exception", []{
        const IOperand* op = OperandFactory::createOperand(static_cast<eOperandType>(999), "42");
        print_result(op);
        delete op;
    }, true);

    banner("8) Operantions with factory-created operands");
    run_case("Factory Int16 + Factory Float => Float", []{
        const IOperand* a = OperandFactory::createOperand(Int16, "12345");
        const IOperand* b = OperandFactory::createOperand(Float, "3.14");
        const IOperand* r = *a + *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    });

    run_case("Factory Double / Factory Int8 => Double", []{
        const IOperand* a = OperandFactory::createOperand(Double, "22.0");
        const IOperand* b = OperandFactory::createOperand(Int8, "7");
        const IOperand* r = *a / *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    });

    run_case("Factory Int32 % Factory Int16 => Int16", []{
        const IOperand* a = OperandFactory::createOperand(Int32, "100");
        const IOperand* b = OperandFactory::createOperand(Int16, "30");
        const IOperand* r = *a % *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    });

    run_case("Factory Float - Factory Double => Double", []{
        const IOperand* a = OperandFactory::createOperand(Float, "5.5");
        const IOperand* b = OperandFactory::createOperand(Double, "2.2");
        const IOperand* r = *a - *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    });

    run_case("Factory Int8 * Factory Int8 => Int8", []{
        const IOperand* a = OperandFactory::createOperand(Int8, "12");
        const IOperand* b = OperandFactory::createOperand(Int8, "10");
        const IOperand* r = *a * *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    });

    run_case("Factory Int8 * Factory Int8 with overflow: expect exception", []{
        const IOperand* a = OperandFactory::createOperand(Int8, "20");
        const IOperand* b = OperandFactory::createOperand(Int8, "10");
        const IOperand* r = *a * *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    });

    run_case("Factory Float / Factory Float div by zero: expect exception", []{
        const IOperand* a = OperandFactory::createOperand(Float, "3.14");
        const IOperand* b = OperandFactory::createOperand(Float, "0.0");
        const IOperand* r = *a / *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    }, true);

    run_case("Factory Double % Factory Double mod by zero: expect exception", []{
        const IOperand* a = OperandFactory::createOperand(Double, "2.71828");
        const IOperand* b = OperandFactory::createOperand(Double, "0.0");
        const IOperand* r = *a % *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    }, true);

    run_case("Factory Int16 - Factory Int32 underflow: expect exception", []{
        const IOperand* a = OperandFactory::createOperand(Int16, "-30000");
        const IOperand* b = OperandFactory::createOperand(Int32, "10000");
        const IOperand* r = *a - *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    });

    run_case("Factory Float + Factory Float overflow: expect exception", []{
        const IOperand* a = OperandFactory::createOperand(Float, "1e38");
        const IOperand* b = OperandFactory::createOperand(Float, "1e38");
        const IOperand* r = *a + *b;
        print_result(r);
        delete a;
        delete b;
        delete r;
    });

    banner("DONE");
    return 0;
}
#endif
