#include "Divi.h"
    /**
     * constructor
     * @param left left exp
     * @param right right exp
     */
Divi::Divi(Expression *left, Expression *right) {
    this->left = left;
    this->right = right;
}
    /**
     * calculate the expresion
     * @return double value of the calculating
     */
double Divi::calculate() {
    double rightE =  this->right->calculate();
    if (rightE == 0) {
        throw "cannot divide by zero";
    }
    return this->left->calculate() / this->right->calculate();
}
    /*
     * @return string value of the div "/"
     */
std::string Divi::getValue() {
    return "/";
}
    /**
     * @return double value of the div
     */
double Divi::getV() {
    return this->calculate();
}