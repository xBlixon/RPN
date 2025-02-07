#pragma once
#include <string>
#include <unordered_set>
#include <sstream>

namespace RPN {
    /**
    * Wrapper over std::stringstream for extracting tokens from the string.
    */
    struct TokenReader {
        /**
         * Token reader constructor
         * @param string Reference to the string from which tokens are read.
         */
        explicit TokenReader(const std::string& string);
        /**
         * Returns the entire string from which reader reads.
         * @return Whole string
         */
        std::string getString();
        /**
         * Next tokens in the stream.
         * @return Next token.
         */
        std::string next();
        /**
         * Checks upcoming token in the stream, while keeping
         * the current position in the stream.
         * @return Upcoming token in the stream.
         */
        std::string peek();
        /**
         * Checks if stream came to an end.
         * @return true if stream has finished.
         */
        bool finished() const;
    private:
        std::string string_;
        std::stringstream stream;
    };

    /**
    * RPN equation solver.
    */
    struct RPNSolver {
        /**
         * Solves for the result of the RPN equation.
         * @return Result of the RPN equation.
         */
        static double getResult(const std::string& equation);
    };

    /**
     * Struct able to convert Infix to RPN and vice versa.
     */
    struct NotationConverter {
        /**
         * Given infix equation string, converts it into RPN equation.
         * @param infix Infix equation
         * @return RPN equation.
         */
        static std::string infixToRPN(const std::string& infix);
        /**
         * Given RPN equation string, converts it into infix equation.
         * @param RPN RPN equation.
         * @return Infix equation.
         */
        static std::string RPNtoInfix(const std::string& RPN);
    private:
        /**
         * Given number a, b and an operator, combines it
         * into single infix string wrapped in parentheses.
         * @param a Left operand.
         * @param b Right operand.
         * @param op Operator.
         * @return String looking as followed: ( a op b )
         */
        static std::string wrapInParentheses(const std::string& a, const std::string& b, const std::string& op);
        /**
         * Given number a, b and an operator, combines it
         * into single infix string.
         * @param a Left operand.
         * @param b Right operand.
         * @param op Operator.
         * @return String looking as followed: a op b
         */
        static std::string aopb(const std::string &a, const std::string &b, const std::string &op);
        static std::string onlyParentheses(const std::string &a);
    };

    struct NotationDeterminer {
        /**
         * Determines if string is an RPN equation.
         * @return true if equation is written in RPN.
         */
        static bool isRPN(const std::string& equation);
        /**
         * Determines if string is an Infix equation.
         * @return true if equation is written in Infix.
         */
        static bool isInfix(const std::string& equation);
    };

    struct Spacer {
        /**
        * Prepares equation to be fed into NotationConverter.
        * Key idea is that
        */
        static std::string addSpacesAroundParentheses(const std::string& input);

        static std::string removeSpacesAroundParentheses(const std::string& input);

        static std::string addSpacesAroundOperators(const std::string& input);

        static std::string removeSpacesAroundOperators(const std::string& input);
    };
}