#include "Minus.h"
    /**
     * constructor
     * @param left left exp
     * @param right right exp
     */
Minus::Minus(Expression *left, Expression *right) {
    this->left = left;
    this->right = right;
}
/**
 * claculate the minus
 * @return result of calculating
 */
double Minus::calculate() {
    return this->left->calculate() - this->right->calculate();
}
/**
 * string value of the -
 * @return -
 */
std::string Minus::getValue() {
    return "-";
}
/**
 * retirn the minus value.
 * @return esult of calculating
 */
double Minus::getV() {
    return this->calculate();
}