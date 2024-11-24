#pragma once
#include <string>

struct RPNEquation {
    explicit RPNEquation(std::string equation);
private:
    std::string equation;
    double result = 0.0f;
    void solve();
};