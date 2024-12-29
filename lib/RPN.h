#pragma once
#include <memory>
#include <string>
#include <unordered_set>
#include <sstream>

namespace RPN {
    /**
    * Wrapper over std::stringstream for extracting tokens from the string.
    */
    struct TokenReader {
        explicit TokenReader(const std::string& string);
        std::string getString();
        std::string next();
        std::string peek();
        bool finished() const;
    private:
        std::string string_;
        std::stringstream stream;
    };

    /**
    * Infix and RPN equation solver.
    */
    struct Equation {
        explicit Equation(const std::string& equation);
        double getResult() const;
    private:
        std::string equation;
        TokenReader reader;
        static bool is1ArgOperator(const std::string& op);
        static bool is2ArgOperator(const std::string& op);
        static const std::unordered_set<std::string> one_arg_operators;
        static const std::unordered_set<std::string> two_arg_operators;
        double result = 0.0f;
        void solve();
    };

    struct NotationConverter {
        static std::string infixToRPN(const std::string& infix);
        static std::string RPNtoInfix(const std::string& RPN);
    private:
        static std::string wrapInParentheses(const std::string& a, const std::string& b, const std::string& op);
        static std::string aopb(const std::string &a, const std::string &b, const std::string &op);
    };

    struct NotationDeterminer {
        static bool isRPN(const std::string& equation);
        static bool isInfix(const std::string& equation);
    };
}