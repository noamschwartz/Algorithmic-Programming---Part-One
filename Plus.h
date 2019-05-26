
#ifndef PROJECT_PLUS_H
#define PROJECT_PLUS_H


#include "BinaryExpression.h"
#include "iostream"

class Plus : public BinaryExpression {
    Expression* left;
    Expression* right;
public:
    Plus(Expression* left, Expression* right);
    virtual double calculate();
    virtual std::string getValue();
    virtual double getV();
};


#endif //PROJECT_PLUS_H