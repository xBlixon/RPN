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
    private:
        std::string string_;
        std::stringstream stream;
    };

    /**
    * Infix and RPN equation solver.
    */
    struct Equation {
        explicit Equation(std::string equation);
        double getResult() const;
    private:
        explicit Equation(const std::shared_ptr<TokenReader>& stream);
        std::string equation;
        std::shared_ptr<TokenReader> stream;
        static bool isOperator(const std::string& op);
        static const std::unordered_set<std::string> operators;
        double result = 0.0f;
        void solve();
    };
}