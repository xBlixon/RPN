#include <iostream>
#include <cstdio>
#include <string>
#include "RPN.h"

int main() {
    std::string strEquation;
    printf("Enter the equation: ");
    std::getline(std::cin, strEquation);
    std::string rpn;
    std::string infix;
    if (RPN::NotationDeterminer::isInfix(strEquation)) {
        infix = std::move(strEquation);
        rpn = RPN::NotationConverter::infixToRPN(infix);
    } else {
        rpn = std::move(strEquation);
        infix = RPN::NotationConverter::RPNtoInfix(rpn);
    }

    double result = RPN::RPNSolver::getResult(rpn);
    std::cout << "RPN representation  : " << rpn    << std::endl;
    std::cout << "Infix representation: " << infix  << std::endl;
    std::cout << "Equation result     : " << result << std::endl;
    return 0;
}