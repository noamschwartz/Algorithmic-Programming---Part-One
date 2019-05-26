
#ifndef PROJECT_NUMBER_H
#define PROJECT_NUMBER_H


#include "Expression.h"

class Number : public Expression {
    double value;
public:
    explicit Number(double num);
    virtual double calculate();
    virtual std::string getValue();
    virtual double getV();
    ~Number(){};
};


#endif //PROJECT_NUMBER_H