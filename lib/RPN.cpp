#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <stack>
#include <unordered_set>
#include <climits>
#include <cmath>
#include "RPN.h"

int sumLetters(const std::string& str) {
    int sum=0;
    for (const auto letter : str) {
        sum+=letter;
    }
    return sum;
}

double handleDivision(const double& a, const double& b) {
    if (b == 0) {
        std::cerr << "[ " << a << "/" << b << " ] - illegal division (Division by zero).\n";
        std::exit(1);
    }
    return a/b;
}

double handleSqrt(const double& a) {
    if (a < 0) {
        std::cerr << "[ sqrt(" << a << ") ] - unsupported root (Root of negative number).\n";
        std::exit(1);
    }
    return std::sqrt(a);
}

double handleCbrt(const double& a) {
    if (a < 0) {
        std::cerr << "[ cbrt(" << a << ") ] - unsupported root (Root of negative number).\n";
        std::exit(1);
    }
    return std::cbrt(a);
}

double calculate(const double& a, const double& b, const std::string& op) {
    switch (op[0]) {
        case '*':
            return a*b;
        case '/':
            return handleDivision(a, b);
        case '\\':
            return handleDivision(b, a);
        case '+':
            return a+b;
        case '-':
            return a-b;
        case '%':
            return std::fmod(a, b);
        case '^':
            return std::pow(a, b);
        default:
            return LONG_MIN;
    }
}

double calculate(const double& a, const std::string& op) {
    /**
     * Integers found in cases of this switch come from
     * the sum of ascii values of letters of the operators.
    */
    switch (sumLetters(op)) {
        case 458: //sqrt
            return handleSqrt(a);
        case 427: //cbrt
            return handleCbrt(a);
        case 310: //abs
            return std::abs(a);
        case 314: //neg
            return -a;
        case 330: //sin
            return std::sin(a);
        case 325: //cos
            return std::cos(a);
        case 323: //tan
            return std::tan(a);
        default:
            return LONG_MIN;
    }
}

namespace RPN {
    const std::unordered_set<std::string> Equation::one_arg_operators = {
        "sqrt",
        "cbrt",
        "abs",
        "neg",
        "sin",
        "cos",
        "tan",
    };

    const std::unordered_set<std::string> Equation::two_arg_operators = {
        "*",
        "/",
        "\\",
        "+",
        "-",
        "%",
        "^"
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
            if (is1ArgOperator(token)) {
                double& a = numbers.top();
                a = calculate(a, token);
            } else if (is2ArgOperator(token)) {
                /**
                 * Takes 2 tokens from the stack,
                 * removing the first and reassigning
                 * the second to the result
                 * of the operation.
                 */
                double b = numbers.top();
                numbers.pop();
                double& a = numbers.top();
                a = calculate(a, b, token);
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
    bool Equation::is1ArgOperator(const std::string& op) {
        return one_arg_operators.count(op) > 0;
    }

    /**
     * Checks if the string is a mathematical operator
     * supported by this struct.
     * @param op String tested for being a valid math operator
     * @return true = A math operator, false = Not a math operator
     */
    bool Equation::is2ArgOperator(const std::string& op) {
        return two_arg_operators.count(op) > 0;
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


