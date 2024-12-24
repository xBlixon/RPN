#pragma once
#include <string>

struct RPNEquation {
    explicit RPNEquation(std::string equation);
private:
    std::string equation;
    static bool isOperator(const std::string& op);
    static std::unordered_set<std::string> operators;
    double result = 0.0f;
    void solve();
};