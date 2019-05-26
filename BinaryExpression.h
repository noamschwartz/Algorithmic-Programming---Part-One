#ifndef PROJECT_BINARYEXPRESSION_H
#define PROJECT_BINARYEXPRESSION_H


#include "Expression.h"
/**
 * This class is abstruct class to the binary expressions +,=,/,*
 */
class BinaryExpression : public Expression {
public:
    /**
     * claculate the expression
     * @return double, result of the calculation
     */
    double calculate() override = 0;
    /**
     * @return string of value
     */
    std::string getValue() override = 0;
    /**
     * @return the double value.
     */
    double getV() override = 0;
    /**
     * Distructor
     */
    ~BinaryExpression() = default;;
};


#endif //PROJECT_BINARYEXPRESSION_H