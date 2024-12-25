#pragma once
#include <string>
#include <unordered_set>
#include <sstream>

namespace RPN {
    struct Equation {
        explicit Equation(std::string equation);
        double getResult() const;
    private:
        std::string equation;
        std::stringstream stream;
        static bool isOperator(const std::string& op);
        static const std::unordered_set<std::string> operators;
        double result = 0.0f;
        void solve();
    };


}