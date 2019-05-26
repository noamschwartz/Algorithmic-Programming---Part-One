#include <iostream>
#include "Expression.h"
#include "Number.h"
#include "Mul.h"
#include "Minus.h"
#include "Plus.h"
#include "Divi.h"
#include "Interpreter.h"
// CPP program to evaluate a given
// expression where tokens are
// separated by space.
//#include <bits/stdc++.h>
using namespace std;



int main(int argc, char* argv[]) {

    string path = argv[1];
    Interpreter* inter = new Interpreter(path);
    inter->lexer();
    return 0;
}
