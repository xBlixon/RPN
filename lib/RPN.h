#pragma once
#include <string>

struct RPNEquation {
    explicit RPNEquation(std::string equation);
private:
    std::string equation;
    static bool isOperator(std::string op);
    double result = 0.0f;
    void solve();
};