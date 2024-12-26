#include <iostream>
#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <stack>
#include <unordered_set>
#include "RPN.h"

double calculate(const double& a, const double& b, const std::string& op) {
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

    /**
     * Entrypoint for solving equations.
     * @param equation String value containing either Infix or RPN equation.
     */
    Equation::Equation(std::string equation) {
        this->equation = std::move(equation);
        this->stream = std::make_shared<TokenReader>(this->equation);
        this->solve();
    }

    /**
     * Private constructor used for recursive solving of equations
     * in parentheses.
     * @param stream Stream from the parent Equation object.
     */
    Equation::Equation(const std::shared_ptr<TokenReader>& stream) {
        this->stream = std::shared_ptr<TokenReader>(stream);
        this->equation = stream->getString();
        this->solve();
    }


    /**
     * Solves the equation based on
     * the equation member of the struct
     * and assigns the result to the
     * result member of the struct.
     */
    void Equation::solve() {
        std::string token;
        std::stack<double> numbers;
        while (!(token = stream->next()).empty()) {
            if (isOperator(token)) {
                /**
                 * Takes 2 tokens from the stack,
                 * removing the first and reassigning
                 * the second to the result
                 * of the operation.
                 */
                double b = numbers.top();
                numbers.pop();
                double& a = numbers.top();
                double result = calculate(a, b, token);
                a = result;
            } else if (token == "(") { // Calculate result for the equation in brackets
                Equation nested_equation(stream);
                numbers.push(nested_equation.getResult());
            } else if (token == ")") { // Finish solving equation in brackets and return result.
                break;
            } else {
                numbers.push(std::stod(token));
            }
        }
        /**
         * After the entire algorithm is done
         * the stack should contain only 1 token,
         * which is equal to the result of the equation.
         */
        result = numbers.top();
    }

    /**
     * Returns the result of the given equation.
     * @return Final result of the equation.
     */
    double Equation::getResult() const {
        return result;
    }

    /**
     * Checks if the string is a mathematical operator
     * supported by this struct.
     * @param op String tested for being a valid math operator
     * @return true = A math operator, false = Not a math operator
     */
    bool Equation::isOperator(const std::string& op) {
        return operators.count(op) > 0;
    }

    /**
     * Token reader constructor
     * @param string Reference to the string from which tokens are read.
     */
    TokenReader::TokenReader(const std::string& string) {
        string_ = string;
        stream = std::stringstream(string);
    }

    /**
     * Next tokens in the stream.
     * @return Next token.
     */
    std::string TokenReader::next() {
        std::string token;
        stream >> token;
        return token;
    }

    /**
     * Returns the entire string from which reader reads.
     * @return Whole string
     */
    std::string TokenReader::getString() {
        return string_;
    }

}


