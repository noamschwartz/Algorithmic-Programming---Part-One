#ifndef PROJECT_DIV_H
#define PROJECT_DIV_H

#include "BinaryExpression.h"

class Mul: public BinaryExpression {
    Expression* left;
    Expression* right;
public:
    Mul(Expression* left, Expression* right);
    virtual double calculate();
    virtual std::string getValue();
    double getV();
    ~Mul(){};
};

#endif //PROJECT_DIV_H