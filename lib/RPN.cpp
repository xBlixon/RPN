#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <stack>
#include <unordered_set>
#include <climits>
#include <cmath>
#include <map>
#include <regex>
#include "RPN.h"

namespace RPN {
    /**
     * Intermediate function used by calculate for 1 parameter
     * operators. Sums ascii values of letters to determine
     * which switch case use.
     * @param str
     * @return Ascii sum of letters.
     */
    int sumLetters(const std::string& str) {
        int sum=0;
        for (const auto letter : str) {
            sum+=letter;
        }
        return sum;
    }

    /**
     * Handler for division. Throws error on divisor = 0.
     * @param a
     * @param b
     * @return a/b
     */
    double handleDivision(const double& a, const double& b) {
        if (b == 0) {
            std::cerr << "[ " << a << "/" << b << " ] - illegal division (Division by zero).\n";
            std::exit(1);
        }
        return a/b;
    }

    /**
     * Calculates square roots and errors on negative numbers.
     * @param a
     * @return sqrt(a)
     */
    double handleSqrt(const double& a) {
        if (a < 0) {
            std::cerr << "[ sqrt(" << a << ") ] - unsupported root (Root of negative number).\n";
            std::exit(1);
        }
        return std::sqrt(a);
    }

    /**
     * Calculates cubic roots and errors on negative numbers.
     * @param a
     * @return cbrt(a)
     */
    double handleCbrt(const double& a) {
        if (a < 0) {
            std::cerr << "[ cbrt(" << a << ") ] - unsupported root (Root of negative number).\n";
            std::exit(1);
        }
        return std::cbrt(a);
    }

    /**
     * Given operator and operands calculates the result
     * @param a left operand
     * @param b right operand
     * @param op operator
     * @return result
     */
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

    /**
     * Given operator and operand calculates the result
     * @param a operand
     * @param op operator
     * @return result
     */
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

    /**
     * Exponential precedence score.
     */
    constexpr int EXP_PREC = 100;
    /**
     * Trigonometric functions precedence score.
     */
    constexpr int TRIG_FUN_PREC = EXP_PREC-1;
    /**
     * Multiplication/division precedence score.
     */
    constexpr int MULT_DIV_PREC = TRIG_FUN_PREC-1;
    /**
     * Addition/subtraction precedence score.
     */
    constexpr int ADD_SUB_PREC = MULT_DIV_PREC-1;

    /**
     * Mapped precedences to operators.
     */
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

    /**
     * Operators taking 1 parameter
     */
    const std::unordered_set<std::string> one_arg_operators = {
        "sqrt",
        "cbrt",
        "sin",
        "cos",
        "tan",
    };

    /**
     * Operators taking 2 parameters
     */
    const std::unordered_set<std::string> two_arg_operators = {
        "^",
        "*",
        "/",
        "\\",
        "+",
        "-"
    };

    /**
     * Checks if given string is a valid operator
     * @param op
     * @return true if string is an operator
     */
    bool isOperator(const std::string& op) {
        return operatorPrecedence.count(op) > 0;
    }

    /**
    * Checks if given token is an operator that takes
    * only 1 argument, e.g. sqrt(x).
    * @return true if is 1 argument operator.
    */
    bool is1ArgOperator(const std::string& op) {
        return one_arg_operators.count(op) > 0;
    }

    /**
    * Checks if given token is an operator that takes
    * 2 arguments, e.g. a + b.
    * @return true if is 2 argument operator.
    */
    bool is2ArgOperator(const std::string& op) {
        return two_arg_operators.count(op) > 0;
    }

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

    std::string NotationConverter::onlyParentheses(const std::string &a) {
        std::string combined = "( ";
        combined.append(a);
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
            if (is1ArgOperator(token)) {
                const std::string& operand = infixStack.top();
                std::string inOperator = token;
                inOperator.append(" ");
                inOperator.append(onlyParentheses(operand));
                infixStack.top() = inOperator;
            } else if (is2ArgOperator(token)) {
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

    std::string Spacer::addSpacesAroundParentheses(const std::string& input) {
        std::string result;
        for (const char ch : input) {
            if (ch == '(' || ch == ')') {
                result += ' ';
                result += ch;
                result += ' ';
            } else {
                result += ch;
            }
        }
        return result;
    }

    std::string Spacer::removeSpacesAroundParentheses(const std::string &input) {
        std::string result = input;

        // Remove all spaces between '('
        result = std::regex_replace(result, std::regex(R"(\s*\(\s*)"), "(");

        // Remove all spaces between ')'
        result = std::regex_replace(result, std::regex(R"(\s*\)\s*)"), ")");

        return result;
    }

    std::string Spacer::addSpacesAroundOperators(const std::string &input) {
        std::string result = input;

        for (const auto& entry : operatorPrecedence) {
            const std::string& key = entry.first;
            std::string pattern;
            // Escape math operators that are regex tokens
            if (key == "+" || key == "*" || key == "\\" || key == "^") {
                pattern = "\\" + key;
            } else {
                pattern = key;
            }
            std::string replacement = " " + key + " ";
            result = std::regex_replace(result, std::regex(pattern), replacement);
        }

        return result;
    }

    std::string Spacer::removeSpacesAroundOperators(const std::string& input) {
        std::string result = input;

        for (const auto& entry : operatorPrecedence) {
            const std::string& key = entry.first;
            std::string pattern;

            // Escape math operators that are regex tokens
            if (key == "+" || key == "*" || key == "\\" || key == "^") {
                pattern = "\\s*\\" + key + "\\s*";
            } else {
                pattern = "\\s*" + key + "\\s*";
            }

            result = std::regex_replace(result, std::regex(pattern), key);
        }

        return result;
    }

    std::string Spacer::mergeSpaces(const std::string &input) {
        return std::regex_replace(input, std::regex("\\s+"), " ");
    }

    std::string Spacer::removeTrailingSpaces(const std::string &input) {
        return std::regex_replace(input, std::regex("\\s+$"), "");
    }

    bool EquationValidator::is_number(const std::string &str) {
        std::istringstream iss(str);
        double d;
        return iss >> std::noskipws >> d && iss.eof();
    }

    bool EquationValidator::isValidRPN(const std::string &equation) {
        std::stack<int> operandStack;
        TokenReader reader(equation);

        while (!reader.finished()) {
            std::string token = reader.next();
            if (token.empty()) break; //End of equation

            if (is1ArgOperator(token)) {
                if (operandStack.size() < 1) return false;
                // Consumes operand and returns to the stack
                // 1 element. Therefore, stack count stays the same.
            }
            else if (is2ArgOperator(token)) {
                if (operandStack.size() < 2) return false;
                operandStack.pop();
                // Consumes 1 more element and returns 1 element back.
                // Therefore, 1 pop is sufficient.
            }
            else {
                try {
                    std::stod(token);
                    operandStack.push(1);
                }
                catch (...) {
                    // Invalid token
                    return false;
                }
            }
        }

        return operandStack.size() == 1;
    }

    bool EquationValidator::isValidInfix(const std::string& equation) {
        TokenReader reader(equation);
        std::stack<std::string> parentheses;
        int operandCount = 0;
        int operatorCount = 0;

        while (!reader.finished()) {
            std::string token = reader.next();
            if (token.empty()) break; //End of equation

            if (token == "(") {
                parentheses.push(token);
            } else if (token == ")") {
                if (parentheses.empty() || parentheses.top() != "(") {
                    return false; // Unbalanced parentheses
                }
                parentheses.pop();
            } else if (is2ArgOperator(token)) {
                operatorCount++;
            } else if (is_number(token)) {
                operandCount++;
            } else if (!is1ArgOperator(token)) {
                return false; // Invalid token
            }
        }

        // Check if parentheses are balanced
        if (!parentheses.empty()) {
            return false;
        }

        if (operandCount != operatorCount + 1) {
            return false;
        }

        return true;
    }

}


