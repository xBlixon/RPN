#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>
#include <utility>
#include <stack>
#include <unordered_set>
#include "RPN.h"

int calculate(int a, int b, std::string op) {
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

const std::unordered_set<std::string> RPNEquation::operators = {
    "*",
    "/",
    "+",
    "-"
};

RPNEquation::RPNEquation(std::string equation) {
    this->equation = std::move(equation);
    this->solve();
}

/**
 * Solves the equation based on
 * the equation member of the struct
 * and assigns the result to the
 * result member of the struct.
 */
void RPNEquation::solve() {
    std::stringstream sstream(this->equation);
    std::string element;
    std::stack<int> numbers;
    while (sstream >> element) {
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

bool RPNEquation::isOperator(const std::string& op) {
    return operators.count(op) > 0;
}
