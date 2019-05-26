#include "Number.h"
/**
 * constructor
 * @param num the value of the number
 */
Number::Number(double num) {
    this->value = num;
}
/**
 * @return the value of the number
 */
double Number::calculate() {
    return this->value;
}
/**
 * returned the string value
 * @return "0"
 */
std::string Number::getValue() {
    return "0";
}
/**
 * @return the value of the number
 */
double Number::getV() {
    return this->value;
}