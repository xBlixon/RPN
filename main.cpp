#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include "RPN.h"
#define DEBUG 0

/**
 * Outputs help when executable has no parameters.
 */
void help() {
    std::cout << "----------\n";
    std::cout << "Usage:\n";
    std::cout << "----------\n";
    std::cout << "-i    Input file\n";
    std::cout << "-o    Output file\n";
    std::cout << "-c    Interactive input\n";
    std::cout << "-r    Use Postfix in output\n";
    std::cout << "----------\n";
    std::cout << "Examples:\n";
    std::cout << "----------\n";
    std::cout << "-io input.txt output.txt\n";
    std::cout << "-oi output.txt input.txt\n";
    std::cout << "-cor interactive_rpn_output.txt\n";
    std::cout << "----------\n";
}

int inputFilePos    = -1;
int outputFilePos   = -1;
bool isInteractive  = false;
bool isRPNOutput    = false;

/**
 * Reads flags and configures their values.
 * @param flags
 */
void setFlags(const std::string &flags) {
    int nonPositionalSkips = 0; //c and r don't have parameters so need to be reduced.
    for (int pos=1; pos<flags.length(); pos++) {
        switch (flags[pos]) {
            case 'i':
                inputFilePos = pos - nonPositionalSkips;
                break;
            case 'o':
                outputFilePos = pos - nonPositionalSkips;
                break;
            case 'c':
                isInteractive = true;
                nonPositionalSkips++;
                break;
            case 'r':
                isRPNOutput = true;
                nonPositionalSkips++;
                break;
            default: break;
        }
    }
}

void solveForOutput(const std::string &sourceEquation, std::string &outputEquation, double &result) {
    std::string rpn;
    std::string infix;
    if (RPN::NotationDeterminer::isInfix(sourceEquation)) {
        infix = sourceEquation;
        rpn = RPN::NotationConverter::infixToRPN(infix);
    } else {
        rpn = sourceEquation;
        infix = RPN::NotationConverter::RPNtoInfix(rpn);
    }
    outputEquation = isRPNOutput ? rpn : infix;
    result = RPN::RPNSolver::getResult(rpn);
}

int main(const int argc, char* argv[]) {
    if (argc < 2) {
        help();
        exit(0);
    }
    setFlags(argv[1]);

    std::string strEquation;
    if (isInteractive) {
        printf("Enter the equation: ");
        std::getline(std::cin, strEquation);
    } else if (inputFilePos != -1) {
        std::ifstream file(argv[inputFilePos+1]);
        std::ostringstream buffer;
        buffer << file.rdbuf();
        strEquation = buffer.str();
    } else {
        std::cerr << "No input source! - Use interactive or file input.";
        exit(1);
    }

    #if DEBUG
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
    #else
        double result;
        std::string outputEquation;
        solveForOutput(strEquation, outputEquation, result);
        std::cout << outputEquation << " = " <<result<<std::endl;
        if (outputFilePos != -1) {
            std::ofstream outputFile(argv[outputFilePos+1], std::ofstream::out);
            outputFile << outputEquation;
            outputFile << " = ";
            outputFile << result;
        }
    #endif
    return 0;
}