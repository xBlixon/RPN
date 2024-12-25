#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <utility>
#include <stack>
#include <unordered_set>
#include "RPN.h"

int calculate(const int& a, const int& b, const std::string& op) {
    switch (op[0]) {
        case '*':
            return a*b;
        case '/':
            return a/b;
        case '+':
            return a+b;
        case '-':
            return a-b;
        default:
            return -2147483648;
    }
}

namespace RPN {
    const std::unordered_set<std::string> Equation::operators = {
        "*",
        "/",
        "+",
        "-"
    };

    Equation::Equation(std::string equation) {
        this->equation = std::move(equation);
        this->stream = std::stringstream(this->equation);
        this->solve();
    }

    /**
     * Solves the equation based on
     * the equation member of the struct
     * and assigns the result to the
     * result member of the struct.
     */
    void Equation::solve() {
        std::string element;
        std::stack<int> numbers;
        while (stream >> element) {
            std::cout << element << std::endl;
            if (isOperator(element)) {
                /**
                 * Takes 2 elements from the stack,
                 * removing the first and reassigning
                 * the second to the result
                 * of the operation.
                 */
                int b = numbers.top();
                numbers.pop();
                int& a = numbers.top();
                int result = calculate(a, b, element);
                a = result;
            } else {
                numbers.push(std::stoi(element));
            }
        }
        /**
         * After the entire algorithm is done
         * the stack should contain only 1 element,
         * which is equal to the result of the equation.
         */
        result = numbers.top();
    }

    /**
     * Returns the result of the given equation.
     */
    double Equation::getResult() const {
        return result;
    }

    /**
     * Checks if the string is a mathematical operator
     * supported by this struct.
     * @param op String tested for being a valid math operator
     * @return
     */
    bool Equation::isOperator(const std::string& op) {
        return operators.count(op) > 0;
    }

    TokenReader::TokenReader(const std::string& string) {
        string_ = string;
        stream = std::stringstream(string);
    }

    std::string TokenReader::next() {
        std::string token;
        stream >> token;
        return token;
    }

    std::string TokenReader::getString() {
        return string_;
    }

}


