#ifndef PROJECT_INTERPETER_H
#define PROJECT_INTERPETER_H

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "Command.h"
#include "OpenServerCommand.h"
#include "ConnectCommand.h"
#include "DefineVarCommand.h"
#include "Expression.h"
#include "LoopCommand.h"
#include "IfCommand.h"
#include "Number.h"
#include "Mul.h"
#include "Minus.h"
#include "Plus.h"
#include "Divi.h"
#include <bits/stdc++.h>
#include <stack>
#include "EqualsCommand.h"
#include "exitCommand.h"
using namespace std;

class Interpreter {
    string filename;
public:
    Interpreter(string name);
    map<string, Command*> makeMap ();
    void parser(vector<string> vector);
    void lexer();
    vector<string> lexserHelp(const vector<string> &originalV);
    void whereTheOp (string s, vector<int> &v);
    bool checkIfOp(char c);
    ~Interpreter(){};
};

#endif // PROJECT_INTERPETER_H