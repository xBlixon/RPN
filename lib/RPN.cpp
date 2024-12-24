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

void RPNEquation::solve() {
    std::stringstream sstream(this->equation);
    std::string element;
    std::stack<int> numbers;
    while (sstream >> element) {
        std::cout << element << std::endl;
        if (isOperator(element)) {
            int a = numbers.top();
            numbers.pop();
            int b = numbers.top();
            numbers.pop();
            std::cout<<calculate(a, b, element);
        } else {
            numbers.push(std::stoi(element));
        }
    }
}

bool RPNEquation::isOperator(const std::string& op) {
    return operators.count(op) > 0;
}
