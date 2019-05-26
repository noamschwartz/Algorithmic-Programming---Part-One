#ifndef PROJECT_EXPRESSION_H
#define PROJECT_EXPRESSION_H

#include "iostream"
class Expression {
public:
    /**
     * calculate the expression
     */
    virtual double calculate() = 0;
    /**
     * @return string value of the exp
     */
    virtual std::string getValue() = 0;
    /**
     * @return double value of the exp
     */
    virtual double getV() = 0;
    /**
     * distructor
     */
     virtual ~Expression(){} ;
};
#endif //PROJECT_EXPRESSION_H