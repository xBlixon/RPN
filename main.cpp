#include <iostream>
#include <cstdio>
#include <string>
#include "RPN.h"

int main() {
    std::string strEquation;
    printf("Please enter the equation:\n");
    std::getline(std::cin, strEquation);
    RPN::Equation equation = RPN::Equation(strEquation);
    std::cout<<"Result: " << equation.getResult()<<std::endl;
    return 0;
}