#pragma once
#include <string>
#include <unordered_set>

struct RPNEquation {
    explicit RPNEquation(std::string equation);
private:
    std::string equation;
    static bool isOperator(const std::string& op);
    static const std::unordered_set<std::string> operators;
    double result = 0.0f;
    void solve();
};