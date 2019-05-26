#include "Plus.h"
    /**
     * constructor
     * @param left left exp
     * @param right right exp
     */
Plus::Plus(Expression *left, Expression *right) {
    this->left = left;
    this->right = right;
}
/**
 * calculating the expression
 * @return value of calculating
 */
double Plus::calculate() {
    return this->left->calculate() + this->right->calculate();
}
/**
 * string value of the +
 * @return +
 */
std::string Plus::getValue() {
    return "+";
}
/**
 * calculating the expression
 * @return value of calculating
 */
double Plus::getV() {
    return this->calculate();
}