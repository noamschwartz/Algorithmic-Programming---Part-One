
#ifndef PROJRCT_DIVI_H
#define PROJRCT_DIVI_H


#include "BinaryExpression.h"
/**
 * class for the div binnary exp
 */
class Divi : public BinaryExpression{
    Expression* left;
    Expression* right;
public:
    /**
     * constructor
     * @param left left exp
     * @param right right exp
     */
    Divi(Expression* left, Expression* right);
    /**
     * calculate the expresion
     * @return double value of the calculating
     */
    double calculate() override;
    /*
     * @return string value of the div "/"
     */
    std::string getValue() override;
    /**
     * @return double value of the div
     */
    double getV() override;
    /**
     * distructor
     */
     ~Divi(){};
};

#endif //PROJRCT_DIVI_H