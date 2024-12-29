#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <stack>
#include <unordered_set>
#include <climits>
#include <cmath>
#include <map>
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

constexpr int EXP_PREC = 100;
constexpr int TRIG_FUN_PREC = EXP_PREC-1;
constexpr int MULT_DIV_PREC = TRIG_FUN_PREC-1;
constexpr int ADD_SUB_PREC = MULT_DIV_PREC-1;

const std::map<std::string, int> operatorPrecedence = {
    {"^", EXP_PREC},
    {"sqrt", EXP_PREC},
    {"cbrt", EXP_PREC},
    {"sin", TRIG_FUN_PREC},
    {"cos", TRIG_FUN_PREC},
    {"tan", TRIG_FUN_PREC},
    {"*", MULT_DIV_PREC},
    {"/", MULT_DIV_PREC},
    {"\\", MULT_DIV_PREC},
    {"+", ADD_SUB_PREC},
    {"-", ADD_SUB_PREC},
};

bool isOperator(const std::string& op) {
    return operatorPrecedence.count(op) > 0;
}

namespace RPN {
    const std::unordered_set<std::string> RPNSolver::one_arg_operators = {
        "sqrt",
        "cbrt",
        "sin",
        "cos",
        "tan",
    };

    const std::unordered_set<std::string> RPNSolver::two_arg_operators = {
        "^",
        "*",
        "/",
        "\\",
        "+",
        "-"
    };

    /**
     * Solves the equation based on
     * the equation member of the struct
     * and assigns the result to the
     * result member of the struct.
     */
    double RPNSolver::getResult(const std::string& equation) {
        TokenReader reader(equation);

        std::string token;
        std::stack<double> numbers;
        while (!(token = reader.next()).empty()) {
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
            } else {
                numbers.push(std::stod(token));
            }
        }
        /**
         * After the entire algorithm is done
         * the stack should contain only 1 token,
         * which is equal to the result of the equation.
         */
        return  numbers.top();
    }

    /**
     * Checks if the string is a mathematical operator
     * supported by this struct.
     * @param op String tested for being a valid math operator
     * @return true = A math operator, false = Not a math operator
     */
    bool RPNSolver::is1ArgOperator(const std::string& op) {
        return one_arg_operators.count(op) > 0;
    }

    /**
     * Checks if the string is a mathematical operator
     * supported by this struct.
     * @param op String tested for being a valid math operator
     * @return true = A math operator, false = Not a math operator
     */
    bool RPNSolver::is2ArgOperator(const std::string& op) {
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

    bool TokenReader::finished() const {
        return stream.eof();
    }

    std::string TokenReader::peek() {
        std::streampos currentPos = stream.tellg();
        std::string next = this->next();
        stream.seekg(currentPos);
        return next;
    }

    std::string NotationConverter::aopb(const std::string &a, const std::string &b, const std::string &op) {
        std::string combined = a;
        combined.append(" ");
        combined.append(op);
        combined.append(" ");
        combined.append(b);
        return combined;
    }

    std::string NotationConverter::wrapInParentheses(const std::string &a, const std::string &b, const std::string &op) {
        std::string combined = "( ";
        std::string aopbStr = aopb(a, b, op);
        combined.append(aopbStr);
        combined.append(" )");
        return combined;
    }

    std::string NotationConverter::infixToRPN(const std::string &infix) {
        std::string equation;
        std::stack<std::string> operators;
        TokenReader reader(infix);

        while (!reader.finished()) {
            std::string token = reader.next();
            if (isOperator(token)) {
                if (!operators.empty() && operators.top() != "(") { // If stack not empty and newest is not (
                    std::string onStack = operators.top();
                    if (operatorPrecedence.at(onStack) >= operatorPrecedence.at(token)) {
                        equation.append(onStack);
                        equation.append(" ");
                        operators.pop();
                    }
                }
                operators.push(token);
            } else if (token == "(") {
                operators.push(token);
            } else if (token == ")") {
                while (operators.top() != "(") {
                    std::string op = operators.top();
                    equation.append(op);
                    equation.append(" ");
                    operators.pop();
                }
                operators.pop();
            } else {
                equation.append(token);
                equation.append(" ");
            }
        }
        while (!operators.empty()) {
            std::string op = operators.top();
            equation.append(op);
            equation.append(" ");
            operators.pop();
        }
        return equation;
    }

    std::string NotationConverter::RPNtoInfix(const std::string &RPN) {
        TokenReader reader(RPN);
        std::stack<std::string> infixStack;
        while (!reader.finished()) {
            std::string token = reader.next();
            if (isOperator(token)) {
                std::string rightOperand = infixStack.top();
                infixStack.pop();
                std::string& leftOperand = infixStack.top();

                bool needsParentheses = false;
                if (!reader.finished()) {
                    std::string nextToken = reader.peek();
                    if (!isOperator(nextToken)) {
                        needsParentheses = true;
                    } else {
                        needsParentheses = operatorPrecedence.at(token) < operatorPrecedence.at(nextToken);
                    }
                }

                leftOperand = needsParentheses
                              ? wrapInParentheses(leftOperand, rightOperand, token)
                              : aopb(leftOperand, rightOperand, token);
            } else {
                infixStack.push(token);
            }
        }
        return infixStack.top();
    }

    bool NotationDeterminer::isRPN(const std::string &equation) {
        TokenReader reader(equation);
        std::string lastToken;
        while (!reader.finished()) {
            lastToken = reader.next();
        }
        // Last token in RPN is always an operator.
        return isOperator(lastToken);
    }

    bool NotationDeterminer::isInfix(const std::string &equation) {
        return !isRPN(equation);
    }

}


