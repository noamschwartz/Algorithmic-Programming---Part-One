#include "Mul.h"
    /**
     * constructor
     * @param left left exp
     * @param right right exp
     */
Mul::Mul(Expression *left, Expression *right) {
    this->left = left;
    this->right = right;
}
    /**
     * calculate the expresion
     * @return double value of the calculating
     */
double Mul::calculate() {
    return this->left->calculate() * this->right->calculate();
}
/**
 * string of the mul
 * @return *
 */
std::string Mul::getValue() {
    return "*";
}
/**
 * @return the value of the calculating
 */
double Mul::getV() {
    return this->calculate();
}