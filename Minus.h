#ifndef PROJECT_MINUS_H
#define PROJECT_MINUS_H

#include "BinaryExpression.h"
class Minus : public BinaryExpression{
    Expression* left;
    Expression* right;
public:
    Minus(Expression* left, Expression* right);
    virtual double calculate();
    virtual std::string getValue();
    virtual double getV();
    ~Minus(){};
};


#endif //PROJECT_MINUS_H